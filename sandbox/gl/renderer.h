#ifndef IND3D_RENDERER_H
#define IND3D_RENDERER_H

#include "graphics.h"
#include "shader.h"

void renderer_init();
void renderer_set_dir_light_model(Model model);
void renderer_set_point_light_model(Model model);

void renderer_add(uint model_vao, ModelInstance instance);
void renderer_add_dir_light_to_shader(DirLight light);
void renderer_add_dir_light_to_scene(DirLight light, ModelInstance instance);
void renderer_add_point_light_to_shader(PointLight light);
void renderer_add_point_light_to_scene(PointLight light, ModelInstance instance);
int renderer_get_point_light_count();

void renderer_clear_model(uint model_vao);
void renderer_clear_all();

void renderer_bind_shader(Shader *shader);
void renderer_bind_light_shader(Shader *shader);

void renderer_render();
void renderer_render_model(Model model);
void renderer_render_lights();

void renderer_free();

#endif