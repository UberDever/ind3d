#include "renderer.h"

static Shader *bound_shader;
static ml_VAO_to_Instance_t renderer_bunch;

static Shader *bound_light_shader;
static Model dir_light_model = {.info = {.vao = -1}};
static Model point_light_model = {.info = {.vao = -1}};
static v_DirLight_t dir_lights;
static v_PointLight_t point_lights;
static v_ModelInstance_t dir_light_instances;
static v_ModelInstance_t point_light_instances;
static uint point_lights_in_shader = 0;

void renderer_init()
{
    mapl_new(v_ModelInstance_t, uint, renderer_bunch, 101);

    vec_new(DirLight, dir_lights, 4);
    vec_new(PointLight, point_lights, 4);
    vec_new(ModelInstance, dir_light_instances, 4);
    vec_new(ModelInstance, point_light_instances, 16);
}

void renderer_set_dir_light_model(Model model)
{
    dir_light_model = model;
}

void renderer_set_point_light_model(Model model)
{
    point_light_model = model;
}

void renderer_add(uint model_vao, ModelInstance instance)
{
    v_ModelInstance_t *instances = mapl_search_ptr(renderer_bunch, &model_vao, sizeof(uint));
    if (instances != NULL)
    {
        vec_push(*instances, instance);
    }
    else
    {
        v_ModelInstance_t vec = {};
        vec_new(ModelInstance, vec, 8);
        vec_push(vec, instance);
        mapl_add(renderer_bunch, &model_vao, sizeof(uint), vec);
    }
}

void renderer_bind_shader(Shader *shader)
{
    bound_shader = shader;
    shader_bind(*bound_shader);
}

void renderer_bind_light_shader(Shader *shader)
{
    bound_light_shader = shader;
    shader_bind(*bound_light_shader);
}

static void bind_model(Model model)
{
    glBindVertexArray(model.info.vao);
    for (int i = 0; i < model.textures.size; i++)
        shader_bind_texture(*bound_shader, model.textures.data[i].id, i);
    if (model.textures.size != 3)
        shader_bind_texture(*bound_shader, 0, 2); // A dirty hack
}

static void prepare_instance(ModelInstance *instance)
{
    const m4_t transform = instance->transform;

    if (shader_has_uniform(*bound_shader, u_model_name))
        shader_set_m4(bound_shader, u_model_name, transform);
    if (shader_has_uniform(*bound_shader, u_iModel_name))
        shader_set_m4(bound_shader, u_iModel_name, m4_transpose(m4_invert_affine(transform)));
}

static void unbind_model()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(0, GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(0, GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(0, GL_TEXTURE_2D);
    glActiveTexture(0);
    glBindVertexArray(0);
}

void renderer_render_model(Model model)
{
    bind_model(model);
    v_ModelInstance_t *instances = mapl_search_ptr(renderer_bunch, &model.info.vao, sizeof(uint));
    for (int j = 0; j < instances->size; j++)
    {
        prepare_instance(&instances->data[j]);
        glDrawElements(GL_TRIANGLES, model.info.idx_count, GL_UNSIGNED_INT, 0);
    }
    unbind_model();
}

void renderer_render()
{
    for (int i = 0; i < renderer_bunch.cap; i++)
    {
        if (renderer_bunch.data[i].is_filled)
        {
            ml_bucket_v_ModelInstance_tuint_t *bucket_ptr = &renderer_bunch.data[i];
            while (bucket_ptr)
            {
                Model model = bucket_ptr->data.data[0].model; //Assuming there are at least one ModelInstance, that represents that model
                bind_model(model);
                v_ModelInstance_t *instances = mapl_search_ptr(renderer_bunch, &model.info.vao, sizeof(uint));
                for (int j = 0; j < instances->size; j++)
                {
                    prepare_instance(&instances->data[j]);
                    glDrawElements(GL_TRIANGLES, model.info.idx_count, GL_UNSIGNED_INT, 0);
                }
                unbind_model();
                bucket_ptr = bucket_ptr->n;
            }
        }
    }
}

void renderer_add_dir_light_to_shader(DirLight light)
{

    shader_set_v3(bound_shader, "u_dirLight.direction", light.direction);
    shader_set_v3(bound_shader, "u_dirLight.properties.ambient",
                  v3_muls(v3_mul(light.properties.ambient, light.properties.color), light.properties.brightness));
    shader_set_v3(bound_shader, "u_dirLight.properties.diffuse",
                  v3_muls(v3_mul(light.properties.diffuse, light.properties.color), light.properties.brightness));
    shader_set_v3(bound_shader, "u_dirLight.properties.specular",
                  v3_muls(v3_mul(light.properties.specular, light.properties.color), light.properties.brightness));
}

void renderer_add_dir_light_to_scene(DirLight light, ModelInstance instance)
{
    vec_push(dir_lights, light);
    vec_push(dir_light_instances, instance);
}

void renderer_add_point_light_to_shader(PointLight light)
{
    char light_name[50] = {};
    sprintf(light_name, "u_pointLights[%d].", point_lights_in_shader);
    point_lights_in_shader++;

    int prev_position = strlen(light_name); // no utf
    strcat(light_name, "position");
    shader_set_v3(bound_shader, light_name, light.position);
    light_name[prev_position] = 0;

    prev_position = strlen(light_name); // no utf
    strcat(light_name, "quadratic");
    shader_set_1f(bound_shader, light_name, light.attenuation.x);
    light_name[prev_position] = 0;

    prev_position = strlen(light_name); // no utf
    strcat(light_name, "linear");
    shader_set_1f(bound_shader, light_name, light.attenuation.y);
    light_name[prev_position] = 0;

    prev_position = strlen(light_name); // no utf
    strcat(light_name, "constant");
    shader_set_1f(bound_shader, light_name, light.attenuation.z);
    light_name[prev_position] = 0;

    prev_position = strlen(light_name); // no utf
    strcat(light_name, "properties.ambient");
    shader_set_v3(bound_shader, light_name,
                  v3_muls(v3_mul(light.properties.ambient, light.properties.color), light.properties.brightness));
    light_name[prev_position] = 0;

    prev_position = strlen(light_name); // no utf
    strcat(light_name, "properties.diffuse");
    shader_set_v3(bound_shader, light_name,
                  v3_muls(v3_mul(light.properties.diffuse, light.properties.color), light.properties.brightness));
    light_name[prev_position] = 0;

    strcat(light_name, "properties.specular");
    shader_set_v3(bound_shader, light_name,
                  v3_muls(v3_mul(light.properties.specular, light.properties.color), light.properties.brightness));
}

void renderer_add_point_light_to_scene(PointLight light, ModelInstance instance)
{
    vec_push(point_lights, light);
    vec_push(point_light_instances, instance);
}

int renderer_get_point_light_count()
{
    return point_lights_in_shader;
}

void renderer_render_lights()
{
    bind_model(dir_light_model);
    for (int i = 0; i < dir_lights.size; i++)
    {
        const m4_t transform = dir_light_instances.data[i].transform;
        shader_set_m4(bound_light_shader, u_model_name, transform);
        shader_set_v3(bound_light_shader, "u_color", v3_muls(dir_lights.data[i].properties.color, dir_lights.data[i].properties.brightness));
        glDrawElements(GL_TRIANGLES, dir_light_model.info.idx_count, GL_UNSIGNED_INT, 0);
    }
    bind_model(point_light_model);
    for (int i = 0; i < point_lights.size; i++)
    {
        const m4_t transform = point_light_instances.data[i].transform;
        shader_set_m4(bound_light_shader, u_model_name, transform);
        shader_set_v3(bound_light_shader, "u_color", v3_muls(point_lights.data[i].properties.color, point_lights.data[i].properties.brightness));
        glDrawElements(GL_TRIANGLES, point_light_model.info.idx_count, GL_UNSIGNED_INT, 0);
    }
    unbind_model();
}

void renderer_clear_all()
{
    for (int i = 0; i < renderer_bunch.cap; i++)
    {
        if (renderer_bunch.data[i].is_filled)
        {
            ml_bucket_v_ModelInstance_tuint_t *ptr = &renderer_bunch.data[i];
            while (ptr)
            {
                v_ModelInstance_t *instances = &ptr->data;
                instances->size = 0;
                ptr = ptr->n;
            }
        }
    }
    dir_light_instances.size = 0;
    point_light_instances.size = 0;
    dir_lights.size = 0;
    point_lights.size = 0;
    point_lights_in_shader = 0;
}

void renderer_clear_model(uint model_vao)
{
    v_ModelInstance_t *instances = mapl_search_ptr(renderer_bunch, &model_vao, sizeof(uint));
    if (instances)
        instances->size = 0;
}

void renderer_free()
{
    for (int i = 0; i < renderer_bunch.cap; i++)
    {
        ml_bucket_v_ModelInstance_tuint_t *ptr = &renderer_bunch.data[i];
        if (ptr->is_filled)
        {
            while (ptr)
            {
                vec_free(ptr->data);
                ptr = ptr->n;
            }
        }
    }
    mapl_free(renderer_bunch);
    vec_free(dir_lights);
    vec_free(point_lights);
    vec_free(dir_light_instances);
    vec_free(point_light_instances);
}
