//
// Created by uberdever on 09.09.2020.
//

#include "enemy.h"

v_Enemy_t enemies;
v_Enemyptr_t enemies_in_chunk;

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

void enemy_init()
{
    vec_new(Enemy, enemies, 32);
    memset(enemies.data, 0, enemies.cap * sizeof(*enemies.data));

    EnemyBehaviour enemy_state = {};
    vec_new(EnemyBehaviour, enemy_behaviours, EnemyState_Size);
    for (int i = 0; i < enemy_behaviours.cap; i++)
    {
        enemy_behaviours.data[i].state = i;
    }
    enemy_behaviours.data[EnemyState_Idle].act = state_idle;
    enemy_behaviours.data[EnemyState_Follow].act = state_follow;
    enemy_behaviours.data[EnemyState_Attack].act = state_attack;

    vec_new(Enemy, enemy_database, EnemyTypes_Size);
    ADD_ENEMY_TYPE(C_ENEMYTYPE_COMMON);
    ADD_ENEMY_TYPE(C_ENEMYTYPE_STRONG);
    ADD_ENEMY_TYPE(C_ENEMYTYPE_BOSS);

    vec_new(Enemyptr, enemies_in_chunk, 64);
}

void enemy_create(v2 pos, enum EnemyTypes type)
{
    Enemy enemy;
    memcpy(&enemy, enemy_database.data + (type - 1), sizeof(Enemy));
    enemy.is_alive = true;
    pi_v2_copy(pos, enemy.pos);
    vec_push(enemies, enemy);
}

void enemy_update()
{
    for (uint i = 0; i < enemies.size; i++)
    {
        if (enemies.data[i].is_alive)
        {
            //Process all projectiles
            //for (uint j = 0; j < vec_get_cap(projectiles); j++)
            //{
            //    v2 rel_pos; pi_v2_sub(enemies[i].pos, projectiles[j].pos, rel_pos);
            //    const float32 hit_radius = enemies[i].hitbox_radius + projectiles[j].radius;
            //    if (pi_v2_len_sq(rel_pos) <= hit_radius * hit_radius)
            //        enemies[i].is_alive = false;
            //}
            // Process states
            enemies.data[i].state->act(&enemies.data[i]);
        }
    }
}

void enemy_render(vi2 offset)
{
}

static bool enemy_in_range(const v2 rel_pos)
{
    float32 k = player.dir[1] / player.dir[0]; // find slope
    float32 range = k * rel_pos[0];            // get corresponding point y position
    range = fabsf(range - rel_pos[1]);         // get non-corrected range
    range *= player.dir[0];                    // assuming that player.dir.x == cos(phi), correct range, so it perpendicular
    if (fabsf(range) <= player.hitscan_range)
    {
        if (pi_v2_dot(player.dir, rel_pos) < 0)
            return true;
    }
    return false;
}

void enemies_process_hit()
{
#if 0
    float32 min_distance = FLT_MAX;
    uint index = 0;
    for (uint i = 0; i < enemies_spotted.size; i++)
    {
        v2 rel_pos;
        pi_v2_sub(player.pos, enemies_spotted.data[i]->pos, rel_pos);
        float32 distance = pi_v2_len(rel_pos);
        if (distance < min_distance)
        {
            min_distance = distance;
            index = i;
        }
    }
    enemies_spotted.data[index]->is_alive = false;
#endif
    float32 min_distance = FLT_MAX;
    uint index = -1;
    for (uint i = 0; i < enemies.size; i++)
    {
        if (enemies.data[i].state->state != EnemyState_Idle &&
            enemies.data[i].state->state != EnemyState_IdleRange)
        {
            v2 rel_pos;
            pi_v2_sub(player.pos, enemies.data[i].pos, rel_pos);
            if (enemy_in_range(rel_pos))
            {
                float32 distance = pi_v2_len(rel_pos);
                if (distance < min_distance)
                {
                    min_distance = distance;
                    index = i;
                }
            }
        }
    }
    debug("Enemy index: %d", index);
    if (index != -1)
        enemies.data[index].is_alive = false;
}
