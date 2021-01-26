//
// Created by uberdever on 07.09.2020.
//

#include "projectile.h"

Model projectile_model;
PointLight projectile_light;

void projectile_init(v_Projectile_t *projectiles)
{
    vec_new(Projectile, *projectiles, 32);
    memset(projectiles->data, 0, projectiles->cap * sizeof(*projectiles->data));

    projectile_model = model_build(generate_sphere(32, 0.5));
    model_add_texture(&projectile_model, texture_load("graphics/textures/plasma.png"));
    model_add_texture(&projectile_model, texture_load("graphics/textures/plasma.png"));
    model_add_texture(&projectile_model, texture_load("graphics/textures/plasma.png"));

    projectile_light = point_light;
    projectile_light.properties.color = C_BLUE;
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
            v3_t projectile_pos = (v3_t){projectiles->data[i].pos.x, 0, projectiles->data[i].pos.y};
            projectiles->data[i].pos = v2_add(projectiles->data[i].pos, projectiles->data[i].momentum);
            projectile_light.position = projectile_pos;
            renderer_add_point_light_to_shader(projectile_light);
            if (tile_is_wall(map, projectiles->data[i].pos.x, projectiles->data[i].pos.y))
                projectiles->data[i].is_alive = false;
        }
    }
}

void projectile_render(v_Projectile_t *projectiles)
{
    for (uint i = 0; i < projectiles->cap; i++)
    {
        if (projectiles->data[i].is_alive)
        {
            v3_t projectile_pos = (v3_t){projectiles->data[i].pos.x, 0, projectiles->data[i].pos.y};
            ModelInstance instance = MI(projectile_model,
                                        transform_instance(projectile_pos,
                                                           (v3_t){0},
                                                           (v3_t){.1, .1, .1}));
            renderer_add(projectile_model.info.vao, instance);
            projectile_light.position = projectile_pos;
            renderer_add_point_light_to_shader(projectile_light);
        }
    }
}

void projectile_clean(v_Projectile_t *projectiles)
{
    vec_free(*projectiles);
}
