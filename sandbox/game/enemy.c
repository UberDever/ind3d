//
// Created by uberdever on 09.09.2020.
//

#include "enemy.h"

Enemy* enemies;
Enemy* enemy_database;

void enemy_init()
{
    Enemy enemy_type; memset(&enemy_type, 0, sizeof(Enemy));
    vec_new(enemy_database, 4);

    enemy_type.type = Common;
    enemy_type.speed = 0.05;
    enemy_type.hitbox_radius = 0.45;
    vec_push(enemy_database, enemy_type);
    memset(&enemy_type, 0, sizeof(Enemy));

    enemy_type.type = Strong;
    enemy_type.speed = 0.05;
    enemy_type.hitbox_radius = 0.45;
    vec_push(enemy_database, enemy_type);
    memset(&enemy_type, 0, sizeof(Enemy));

    enemy_type.type = Boss;
    enemy_type.speed = 0.05;
    enemy_type.hitbox_radius = 0.45;
    vec_push(enemy_database, enemy_type);

}

void enemy_create(v2 pos, enum EnemyTypes type)
{
    Enemy enemy;
    memcpy(&enemy, enemy_database + (type - 1), sizeof(Enemy));
    enemy.is_alive = true;
    pi_v2_copy(pos, enemy.pos);
    vec_push(enemies, enemy);
}

void enemy_update() {
    for (uint i = 0; i < vec_get_cap(enemies); i++)
    {
        if (enemies[i].is_alive)
        {
            v2 rel_pos; pi_v2_sub(enemies[i].pos, player.pos, rel_pos);
            float32 cross_left = pi_v2_cross(player.left_ray, rel_pos);
            float32 cross_right = pi_v2_cross(rel_pos, player.right_ray);
            enemies[i].is_active =  (cross_left >= 0 && cross_right >= 0);
            //debug("%lf %lf", cross_left, cross_right);
            //g_screen_draw_line(g_scr.w / 2, g_scr.h / 2, (g_scr.w / 2) + rel_pos[0] * map.tile_w, (g_scr.h / 2) + rel_pos[1] * map.tile_h, COLOR(yellow));

            for (uint j = 0; j < vec_get_cap(projectiles); j++)
            {
                v2 rel_distance; pi_v2_sub(enemies[i].pos, projectiles[j].pos, rel_distance);
                const float32 hit_radius = enemies[i].hitbox_radius + projectiles[j].radius;
                if (pi_v2_len_sq(rel_distance) <= hit_radius * hit_radius)
                    enemies[i].is_alive = false;
            }
            rel_pos[0] = -rel_pos[0]; rel_pos[1] = -rel_pos[1];
            pi_v2_normalize(rel_pos, rel_pos);
            pi_v2_muls(rel_pos, enemies[i].speed, enemies[i].momentum);
            //v2 step_left, step_right; pi_v2_copy(enemies[i].momentum, step_left); pi_v2_copy(enemies[i].momentum, step_right);
            //pi_v2_orthogonal_left(step_left, step_left);
            //pi_v2_orthogonal_right(step_right, step_right);

            v2 test_vector = {};
            test_vector[0] = enemies[i].momentum[0] >= 0 ? enemies[i].hitbox_radius : -enemies[i].hitbox_radius;
            test_vector[1] = enemies[i].momentum[1] >= 0 ? enemies[i].hitbox_radius : -enemies[i].hitbox_radius;
            v2 test_position; pi_v2_add(enemies[i].pos, enemies[i].momentum, test_position);
            if (map.data[(int)test_position[1] * map.w + (int)(test_position[0] + test_vector[0])])
                enemies[i].momentum[0] = 0;
            if (map.data[(int)(test_position[1] + test_vector[1]) * map.w + (int)test_position[0]])
                enemies[i].momentum[1] = 0;
            pi_v2_muls(test_vector, SQRT2_2, test_vector);
            if (map.data[(int)(test_position[1] - test_vector[1]) * map.w + (int)(test_position[0] + test_vector[0])])
                enemies[i].momentum[0] = 0;
            if (map.data[(int)(test_position[1] + test_vector[1]) * map.w + (int)(test_position[0] - test_vector[0])])
                enemies[i].momentum[1] = 0;
            if (map.data[(int)(test_position[1] + test_vector[1]) * map.w + (int)(test_position[0] + test_vector[0])])
            {
                enemies[i].momentum[0] = 0;
                enemies[i].momentum[1] = 0;
            }
            pi_v2_add(enemies[i].pos, enemies[i].momentum, enemies[i].pos);
        }
    }
}

void enemy_render(vi2 offset) {
    offset[0] -= player.pos[0] * map.tile_w; offset[1] -= player.pos[1] * map.tile_h;
    for (uint i = 0; i < vec_get_cap(enemies); i++)
    {
        if (enemies[i].is_alive)
        {
            const int rad = enemies[i].hitbox_radius * map.tile_w;
            v2 screen_pos = { offset[0] + enemies[i].pos[0] * map.tile_w, offset[1] + enemies[i].pos[1] * map.tile_h};
            if (screen_pos[0] > rad && screen_pos[1] > rad &&
                screen_pos[0] < (g_scr.w - rad) && screen_pos[1] < (g_scr.h - rad))
                g_screen_draw_circle(screen_pos, rad, enemies[i].is_active ? COLOR(red) : COLOR(light_gray));
        }
    }
}
