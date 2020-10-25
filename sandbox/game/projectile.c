//
// Created by uberdever on 07.09.2020.
//

#include "projectile.h"

void projectile_init(v_Projectile_t *projectiles)
{
    vec_new(Projectile, *projectiles, 32);
    memset(projectiles->data, 0, projectiles->cap * sizeof(*projectiles->data));
}

void projectile_create(v_Projectile_t *projectiles, v2 pos, v2 dir)
{
    Projectile prj = {.momentum = {0}, .radius = C_PROJECTILE_RADIUS, .speed = C_PROJECTILE_SPEED, .is_alive = true};
    pi_v2_copy(pos, prj.pos);
    pi_v2_muls(dir, prj.speed, prj.momentum);
    for (uint i = 0; i < projectiles->cap; i++)
    {
        if (projectiles->data[i].is_alive == false)
        {
            projectiles->data[i] = prj;
            break;
        }
    }
}

void projectile_update(Map *map, v_Projectile_t *projectiles)
{
    for (uint i = 0; i < projectiles->cap; i++)
    {
        if (projectiles->data[i].is_alive)
        {
            pi_v2_add(projectiles->data[i].pos, projectiles->data[i].momentum, projectiles->data[i].pos);
            if (tile_is_wall(map, projectiles->data[i].pos[0], projectiles->data[i].pos[1]))
                projectiles->data[i].is_alive = false;
        }
    }
}

void projectile_render(v_Projectile_t *projectiles, vi2 offset)
{
}

void projectile_clean(v_Projectile_t *projectiles)
{
    vec_free(*projectiles);
}
