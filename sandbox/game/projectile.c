//
// Created by uberdever on 07.09.2020.
//

#include "projectile.h"

Projectile* projectiles;

static const float32 proj_radius = 0.3f;
static const float32 proj_speed = 0.1;

void projectile_create(v2 pos, v2 dir) {
    Projectile prj = {.momentum = {0}, .radius = proj_radius, .speed = proj_speed, .is_alive = true};
    pi_v2_copy(pos, prj.pos);
    pi_v2_muls(dir, prj.speed, prj.momentum);
    for (uint i = 0; i < vec_get_cap(projectiles); i++)
    {
        if (projectiles[i].is_alive == false)
        {
            projectiles[i] = prj;
            break;
        }
    }
}

void projectile_update() {
    for (uint i = 0; i < vec_get_cap(projectiles); i++)
    {
        if (projectiles[i].is_alive)
        {
            pi_v2_add(projectiles[i].pos, projectiles[i].momentum, projectiles[i].pos);
            if (map.data[(int)projectiles[i].pos[1] * map.w + (int)projectiles[i].pos[0]])
                projectiles[i].is_alive = false;
        }
    }
}

void projectile_render(vi2 offset) {
    const int rad = proj_radius * map.tile_w;
    offset[0] -= player.pos[0] * map.tile_w; offset[1] -= player.pos[1] * map.tile_h;
    for (uint i = 0; i < vec_get_cap(projectiles); i++)
    {
        if (projectiles[i].is_alive)
        {
            pi_screen_draw_circle((v2){
                                    offset[0] + projectiles[i].pos[0] * map.tile_w,
                                    offset[1] + projectiles[i].pos[1] * map.tile_h},
                                    rad, COLOR(blue));
        }
    }
}
