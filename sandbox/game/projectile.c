//
// Created by uberdever on 07.09.2020.
//

#include "projectile.h"

void projectile_init(v_Projectile_t *projectiles)
{
    vec_new(Projectile, *projectiles, 32);
    memset(projectiles->data, 0, projectiles->cap * sizeof(*projectiles->data));
}

void projectile_create(v_Projectile_t *projectiles, v2_t pos, v2_t dir)
{
    Projectile prj = {.momentum = {0}, .radius = C_PROJECTILE_RADIUS, .speed = C_PROJECTILE_SPEED, .is_alive = true};
    prj.pos = pos;
    prj.momentum = v2_muls(dir, prj.speed);
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
            projectiles->data[i].pos = v2_add(projectiles->data[i].pos, projectiles->data[i].momentum);
            if (tile_is_wall(map, projectiles->data[i].pos.x, projectiles->data[i].pos.y))
                projectiles->data[i].is_alive = false;
        }
    }
}

void projectile_clean(v_Projectile_t *projectiles)
{
    vec_free(*projectiles);
}
