//
// Created by uberdever on 09.09.2020.
//

#include "enemy.h"
#include "enemy_states.h"

v_Enemy_t enemy_database;
v_EnemyBehaviour_t enemy_behaviours;

#define ADD_ENEMY_TYPE(T)                                           \
    {                                                               \
        Enemy enemy_type = {};                                      \
        ARR_COPY_LIST(enemy_type, (T));                             \
        enemy_type.state = &enemy_behaviours.data[EnemyState_Idle]; \
        enemy_type.hp = enemy_type.max_hp;                          \
        vec_push(enemy_database, enemy_type);                       \
    }

void enemies_init(v_Enemy_t *enemies, Map *map)
{
    vec_new(Enemy, *enemies, 32);
    memset(enemies->data, 0, enemies->cap * sizeof(*enemies->data));

    EnemyBehaviour enemy_state = {};
    vec_new(EnemyBehaviour, enemy_behaviours, EnemyState_Size);
    for (int i = 0; i < enemy_behaviours.cap; i++)
    {
        enemy_behaviours.data[i].state = (EnemyState)i;
    }
    enemy_behaviours.data[EnemyState_Idle].act = state_idle;
    enemy_behaviours.data[EnemyState_Follow].act = state_follow;
    enemy_behaviours.data[EnemyState_Attack].act = state_attack;

    vec_new(Enemy, enemy_database, EnemyTypes_Size);
    memset(enemy_database.data, 0, enemy_database.cap * sizeof(*enemy_database.data));
    {
        Enemy enemy_type = {};
        ARR_COPY_LIST(enemy_type, (C_ENEMYTYPE_COMMON));
        enemy_type.state = &enemy_behaviours.data[EnemyState_Idle];
        enemy_type.hp = enemy_type.max_hp;
        model_add_texture(&enemy_type.model, texture_load("graphics/sprites/robot_all.png"));
        model_add_texture(&enemy_type.model, texture_load("graphics/sprites/robot_all.png"));
        vec_push(enemy_database, enemy_type);
    }
    {
        Enemy enemy_type = {};
        ARR_COPY_LIST(enemy_type, (C_ENEMYTYPE_STRONG));
        enemy_type.state = &enemy_behaviours.data[EnemyState_Idle];
        enemy_type.hp = enemy_type.max_hp;
        model_add_texture(&enemy_type.model, texture_load("graphics/sprites/robot_all.png"));
        model_add_texture(&enemy_type.model, texture_load("graphics/sprites/robot_all.png"));
        vec_push(enemy_database, enemy_type);
    }
    {
        Enemy enemy_type = {};
        ARR_COPY_LIST(enemy_type, (C_ENEMYTYPE_BOSS));
        enemy_type.state = &enemy_behaviours.data[EnemyState_Idle];
        enemy_type.hp = enemy_type.max_hp;
        model_add_texture(&enemy_type.model, texture_load("graphics/sprites/robot_all.png"));
        model_add_texture(&enemy_type.model, texture_load("graphics/sprites/robot_all.png"));
        vec_push(enemy_database, enemy_type);
    }

    for (int i = 0; i < map->h; i++)
    {
        for (int j = 0; j < map->w; j++)
        {
            if (map->data[i * map->w + j] == 'R' - '0')
            {
                enemies_create(enemies, (v2_t){j + .5, i + .5}, EnemyTypes_Common);
            }
        }
    }
}

void enemies_create(v_Enemy_t *enemies, v2_t pos, enum EnemyTypes type)
{
    Enemy enemy;
    memcpy(&enemy, &enemy_database.data[type - 1], sizeof(Enemy));
    enemy.is_alive = true;
    enemy.pos = pos;
    vec_push(*enemies, enemy);
}

void enemies_update(Map *map, Player *player, v_Enemy_t *enemies, v_Projectile_t *projectiles)
{
    for (uint i = 0; i < enemies->size; i++)
    {
        if (enemies->data[i].is_alive)
        {
            //Process all projectiles
            for (uint j = 0; j < projectiles->cap; j++)
            {
                v2_t rel_pos = v2_sub(enemies->data[i].pos, projectiles->data[j].pos);
                const float32 hit_radius = enemies->data[i].hitbox_radius + projectiles->data[j].radius;
                if (v2_len_sq(rel_pos) <= hit_radius * hit_radius)
                    enemies->data[i].is_alive = false;
            }
            enemies->data[i].state_frame = enemies->data[i].state_frame < 10000 ? enemies->data[i].state_frame + 1 : 0;
            // Process states
            if (near_player(enemies->data[i].pos, player->pos, C_CHUNK_DIST))
                enemies->data[i].state->act(map, player, &enemies->data[i]);
        }
    }
}

static bool enemy_in_range(Player *player, const v2_t rel_pos)
{
    float32 k = player->camera.dir.z / player->camera.dir.x; // find slope
    float32 range = k * rel_pos.x;                           // get corresponding point y position
    range = fabsf(range - rel_pos.y);                        // get non-corrected range
    range *= player->camera.dir.x;                           // assuming that player->dir.x == cos(phi), correct range, so it perpendicular
    if (fabsf(range) <= player->hitscan_range)
    {
        if (v2_dot((v2_t){player->camera.dir.x, player->camera.dir.z}, rel_pos) < 0)
            return true;
    }
    return false;
}

void enemies_process_hit(Map *map, Player *player, v_Enemy_t *enemies)
{
    float32 min_distance = FLT_MAX;
    uint index = -1;
    for (uint i = 0; i < enemies->size; i++)
    {
        if (enemies->data[i].state->state != EnemyState_Idle &&
            enemies->data[i].state->state != EnemyState_IdleRange)
        {
            v2_t rel_pos = v2_sub(player->pos, enemies->data[i].pos);
            if (enemy_in_range(player, rel_pos))
            {
                int wall_x = -1, wall_y = -1;
                linecast(map, enemies->data[i].pos.x, enemies->data[i].pos.y, player->pos.x, player->pos.y, &wall_x, &wall_y, tile_is_wall);
                if (wall_x == -1 && wall_y == -1)
                {
                    float32 distance = v2_len(rel_pos);
                    if (distance < min_distance)
                    {
                        min_distance = distance;
                        index = i;
                    }
                }
            }
        }
    }
    if (index != -1)
    {
        enemies->data[index].is_alive = false;
        is_all_enemies_dead = true;
        for (int i = 0; i < enemies->size; i++)
        {
            is_all_enemies_dead &= !enemies->data[i].is_alive;
        }
    }
}

void enemies_render(v_Enemy_t *enemies)
{
    for (uint i = 0; i < enemies->size; i++)
    {
        if (enemies->data[i].is_alive)
        {
            float ang = acosf(v2_dot(enemies->data[i].dir_to_player, (v2_t){0, 1}));
            ang = enemies->data[i].dir_to_player.x > 0 ? ang : 2 * M_PI - ang;
            ModelInstance instance = MI(enemies->data[i].model, transform_instance((v3_t){enemies->data[i].pos.x, -0.1, enemies->data[i].pos.y},
                                                                                   (v3_t){0, ang, 0},
                                                                                   (v3_t){.1, .1, .1}));

            renderer_add(enemies->data[i].model.info.vao, instance);
        }
    }
}

void enemies_clean(v_Enemy_t *enemies)
{
    vec_free(*enemies);
    vec_free(enemy_database);
    vec_free(enemy_behaviours);
}
