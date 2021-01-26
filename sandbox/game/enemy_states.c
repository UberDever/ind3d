#include "enemy_states.h"

static bool enemy_is_hit(int x, int y)
{
    // const int index = y * map.w + x;
    // return map.data[index] != 0;
    return false;
}

void state_idle(Map *map, Player *player, Enemy *enemy)
{
    v2_t rel_pos = v2_sub(enemy->pos, player->pos);
    //debug_v2(player->pos);
    //rel_pos = v2_muls(rel_pos, -1);
    // Check this every even frame
    if (frames % 2 == 0)
    {
        // Check enemy in FOV
        float32 cross_left = v2_area(player->left_frustum, rel_pos);
        float32 cross_right = v2_area(rel_pos, player->right_frustum);
        //debug("%lf %lf", cross_left, cross_right);
        //line_renderer_add(g_scr.w / 2, g_scr.h / 2, (g_scr.w / 2) + rel_pos.x * map->tile_w, (g_scr.h / 2) + rel_pos.y * map->tile_h, COLOR(yellow));

        if (cross_left >= 0 && cross_right >= 0)
        {
            // Get enemy_to_player direction
            rel_pos.x = -rel_pos.x;
            rel_pos.y = -rel_pos.y;
            // Calculate direction
            enemy->len_to_player = v2_len(rel_pos);
            enemy->dir_to_player = v2_divs(rel_pos, enemy->len_to_player);
            // Check if enemy really see player
            int wall_x = -1, wall_y = -1;
            linecast(map, enemy->pos.x, enemy->pos.y, player->pos.x, player->pos.y, &wall_x, &wall_y, tile_is_wall);
            if (wall_x == -1 && wall_y == -1)
            {
                enemy->state_frame = 0;
                enemy->state = &enemy_behaviours.data[EnemyState_Follow];
            }
        }
    }
}

void state_follow(Map *map, Player *player, Enemy *enemy)
{
    v2_t rel_pos = v2_sub(player->pos, enemy->pos);
    // Calculate momentum
    enemy->len_to_player = v2_len(rel_pos);
    enemy->dir_to_player = v2_divs(rel_pos, enemy->len_to_player);
    enemy->momentum = v2_muls(enemy->dir_to_player, enemy->speed);
    // Collision
    v2_t test_vector = {};
    test_vector.x = enemy->momentum.x >= 0 ? enemy->hitbox_radius : -enemy->hitbox_radius;
    test_vector.y = enemy->momentum.y >= 0 ? enemy->hitbox_radius : -enemy->hitbox_radius;
    v2_t test_position = v2_add(enemy->pos, enemy->momentum);
    if (tile_is_wall(map, test_position.x + test_vector.x, test_position.y))
        enemy->momentum.x = 0;
    if (tile_is_wall(map, test_position.x, test_position.y + test_vector.y))
        enemy->momentum.y = 0;
    test_vector = v2_muls(test_vector, SQRT2_2);
    if (tile_is_wall(map, test_position.x + test_vector.x, test_position.y - test_vector.y))
        enemy->momentum.x = 0;
    if (tile_is_wall(map, test_position.x - test_vector.x, test_position.y + test_vector.y))
        enemy->momentum.y = 0;
    if (tile_is_wall(map, test_position.x + test_vector.x, test_position.y + test_vector.y))
    {
        enemy->momentum.x = 0;
        enemy->momentum.y = 0;
    }
    // Movement
    enemy->pos = v2_add(enemy->pos, enemy->momentum);
    //debug_v2(enemy->momentum);

    if (enemy->state_frame >= 600)
    {
        enemy->state_frame = 0;
        enemy->state = &enemy_behaviours.data[EnemyState_Idle];
    }
    if (enemy->len_to_player <= player->hitbox_radius + enemy->hitbox_radius)
    {
        enemy->state_frame = 0;
        enemy->state = &enemy_behaviours.data[EnemyState_Attack];
    }
}

void state_attack(Map *map, Player *player, Enemy *enemy)
{
    v2_t rel_pos = v2_sub(player->pos, enemy->pos);
    // Calculate  direction
    enemy->len_to_player = v2_len(rel_pos);
    enemy->dir_to_player = v2_divs(rel_pos, enemy->len_to_player);
    // TODO: make this parametrized
    if (enemy->state_frame % 10 == 0)
    {
        player->hp -= enemy->damage;
        if (player->hp <= 0)
            is_player_dead = true;
    }
    if (enemy->len_to_player > player->hitbox_radius + enemy->hitbox_radius)
    {
        enemy->state_frame = 0;
        enemy->state = &enemy_behaviours.data[EnemyState_Follow];
    }
}
