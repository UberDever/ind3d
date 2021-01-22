#ifndef IND3D_SHADER_H
#define IND3D_SHADER_H

#include "graphics.h"

#define UNIFORM_NAME_SIZE 80

typedef GLuint shader_t;
typedef struct Uniform
{
    GLchar name[UNIFORM_NAME_SIZE];
    GLint location;
} Uniform;
vec_register(Uniform);

typedef struct Shader
{
    shader_t program;
    v_Uniform_t uniforms;
    v_GLuint_t samplers_location;
} Shader;

Shader shader_init(const char *vertex_file_path, const char *fragment_file_path);
Shader shader_init_str(const char *vertex_text, const char *fragment_text);

static inline void shader_bind(Shader shader)
{
    glUseProgram(shader.program);
}

static inline void shader_unbind()
{
    glUseProgram(0);
}

static inline void shader_clean(Shader shader)
{
    shader_unbind();
    glDeleteProgram(shader.program);
    vec_free(shader.uniforms);
}

static inline bool shader_has_uniform(Shader shader, const GLchar *name)
{
    if (glGetUniformLocation(shader.program, name) != -1)
        return true;
    return false;
}

static inline GLint uniform_get_location(Shader shader, Uniform uniform)
{
    GLint location = glGetUniformLocation(shader.program, uniform.name);
    if (location == -1)
        warning("Cannot get uniform %s", uniform.name);
    return location;
}

static inline Uniform uniform_init(Shader shader, const GLchar *name)
{
    Uniform uniform = {.location = -1};
    strncpy(uniform.name, name, UNIFORM_NAME_SIZE);
    uniform.location = uniform_get_location(shader, uniform);
    return uniform;
}

static inline void shader_set_1i(Shader *shader, const char *name, int integer)
{
    //
    //glGetUniformLocation(shader->program, name);

    for (uint i = 0; i < shader->uniforms.size; i++)
    {
        if (strcmp(shader->uniforms.data[i].name, name) == 0)
        {
            glUniform1i((GLint)shader->uniforms.data[i].location, integer);
            return;
        }
    }
    vec_push(shader->uniforms, uniform_init(*shader, name));
    glUniform1i(vec_back(shader->uniforms).location, integer);
    int a = 0;
}

static inline void shader_set_1f(Shader *shader, const char *name, float f)
{
    for (uint i = 0; i < shader->uniforms.size; i++)
    {
        if (strcmp(shader->uniforms.data[i].name, name) == 0)
        {
            glUniform1f(shader->uniforms.data[i].location, f);
            return;
        }
    }
    vec_push(shader->uniforms, uniform_init(*shader, name));
    glUniform1f(vec_back(shader->uniforms).location, f);
}

static inline void shader_set_v3(Shader *shader, const char *name, v3_t v)
{
    //glUniform3f(glGetUniformLocation(shader->program, name), v.x, v.y, v.z);
    for (uint i = 0; i < shader->uniforms.size; i++)
    {
        if (strcmp(shader->uniforms.data[i].name, name) == 0)
        {
            glUniform3f(shader->uniforms.data[i].location, v.x, v.y, v.z);
            //debug_v3(v);
            //debug("%d %s", shader->uniforms.data[i].location, shader->uniforms.data[i].name);
            return;
        }
    }
    vec_push(shader->uniforms, uniform_init(*shader, name));
    glUniform3f(vec_back(shader->uniforms).location, v.x, v.y, v.z);
}

static inline void shader_set_m4(Shader *shader, const char *name, m4_t mat)
{
    for (uint i = 0; i < shader->uniforms.size; i++)
    {
        if (strcmp(shader->uniforms.data[i].name, name) == 0)
        {
            glUniformMatrix4fv(shader->uniforms.data[i].location, 1, GL_FALSE, (const GLfloat *)mat.m);
            return;
        }
    }
    vec_push(shader->uniforms, uniform_init(*shader, name));
    glUniformMatrix4fv(vec_back(shader->uniforms).location, 1, GL_FALSE, (const GLfloat *)mat.m);
}

static inline void shader_find_samplers2D(Shader *shader, v_pConstChar_t samplers_names)
{
    for (int i = 0; i < samplers_names.size; i++)
    {
        Uniform uniform = {};
        strncpy(uniform.name, samplers_names.data[i], UNIFORM_NAME_SIZE);
        vec_push(shader->samplers_location, uniform_get_location(*shader, uniform));
    }
}

static inline void shader_bind_texture(Shader shader, texture_t texture, int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(shader.samplers_location.data[slot], slot);
}
#endif