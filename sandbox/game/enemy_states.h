#ifndef IND3D_ENEMY_STATES_H
#define IND3D_ENEMY_STATES_H

#include "../util.h"

static bool enemy_is_hit(int x, int y)
{
    const int index = y * map.w + x;
    return map.data[index] != 0;
}

static void state_idle(Enemy *enemy)
{
    v2 rel_pos;
    pi_v2_sub(enemy->pos, player.pos, rel_pos);
    // Check this every even frame
    if (frames % 2 == 0)
    {
        // Check enemy in FOV
        float32 cross_left = pi_v2_cross(player.left_frustum_ray, rel_pos);
        float32 cross_right = pi_v2_cross(rel_pos, player.right_frustum_ray);
        //debug("%lf %lf", cross_left, cross_right);
        //g_screen_draw_line(g_scr.w / 2, g_scr.h / 2, (g_scr.w / 2) + rel_pos[0] * map.tile_w, (g_scr.h / 2) + rel_pos[1] * map.tile_h, COLOR(yellow));

        if (cross_left >= 0 && cross_right >= 0)
        {
            // Get enemy_to_player direction
            rel_pos[0] = -rel_pos[0];
            rel_pos[1] = -rel_pos[1];
            enemy->len_to_player = pi_v2_normalize(rel_pos, enemy->dir_to_player);
            // Check if enemy really see player
            int wall_x = -1, wall_y = -1;
            linecast(enemy->pos[0], enemy->pos[1], player.pos[0], player.pos[1], &wall_x, &wall_y, util_tile_is_wall);
            if (wall_x == -1 && wall_y == -1)
            {
                enemy->state_frame = 0;
                enemy->state = &enemy_behaviours.data[EnemyState_Follow];
            }
        }
    }
}

static void state_follow(Enemy *enemy)
{
    v2 rel_pos;
    pi_v2_sub(player.pos, enemy->pos, rel_pos);
    // Recalculate direction every fifth frame
    if (frames % 5 == 0)
    {
        // Calculate momentum
        enemy->len_to_player = pi_v2_normalize(rel_pos, enemy->dir_to_player);
        pi_v2_muls(enemy->dir_to_player, enemy->speed, enemy->momentum);
    }
    // Collision
    v2 test_vector = {};
    test_vector[0] = enemy->momentum[0] >= 0 ? enemy->hitbox_radius : -enemy->hitbox_radius;
    test_vector[1] = enemy->momentum[1] >= 0 ? enemy->hitbox_radius : -enemy->hitbox_radius;
    v2 test_position;
    pi_v2_add(enemy->pos, enemy->momentum, test_position);
    if (util_tile_is_wall(test_position[0] + test_vector[0], test_position[1]))
        enemy->momentum[0] = 0;
    if (util_tile_is_wall(test_position[0], test_position[1] + test_vector[1]))
        enemy->momentum[1] = 0;
    pi_v2_muls(test_vector, SQRT2_2, test_vector);
    if (util_tile_is_wall(test_position[0] + test_vector[0], test_position[1] - test_vector[1]))
        enemy->momentum[0] = 0;
    if (util_tile_is_wall(test_position[0] - test_vector[0], test_position[1] + test_vector[1]))
        enemy->momentum[1] = 0;
    if (util_tile_is_wall(test_position[0] + test_vector[0], test_position[1] + test_vector[1]))
    {
        enemy->momentum[0] = 0;
        enemy->momentum[1] = 0;
    }
    pi_v2_add(enemy->pos, enemy->momentum, enemy->pos);

    enemy->state_frame++;

    if (enemy->state_frame >= 600)
    {
        enemy->state_frame = 0;
        enemy->state = &enemy_behaviours.data[EnemyState_Idle];
    }

    if (enemy->len_to_player <= player.hitbox_radius + enemy->hitbox_radius)
    {
        enemy->state_frame = 0;
        enemy->state = &enemy_behaviours.data[EnemyState_Attack];
    }
}

static void state_attack(Enemy *enemy)
{
    v2 rel_pos;
    pi_v2_sub(player.pos, enemy->pos, rel_pos);
    // Recalculate direction every fifth frame
    if (frames % 5 == 0)
    {
        enemy->len_to_player = pi_v2_normalize(rel_pos, enemy->dir_to_player);
    }
    // TODO: make this parametrized
    if (enemy->state_frame % 10 == 0)
    {
        player.hp--;
    }

    enemy->state_frame = enemy->state_frame < 60 ? enemy->state_frame + 1 : 0;
    if (enemy->len_to_player > player.hitbox_radius + enemy->hitbox_radius)
    {
        enemy->state_frame = 0;
        enemy->state = &enemy_behaviours.data[EnemyState_Follow];
    }
}

#endif