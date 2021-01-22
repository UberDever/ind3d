#include "plane_renderer.h"

#define MAX_TEXTURES 256
#define STRIDE 4

static const char *vertex_shader = "\
#version 330 core\n\
\
    layout(location = 0) in vec2 a_position;\
    layout(location = 1) in vec2 a_texCoords;\
\
    out vec2 v_texCoords;\
    \
void main()\
{\
    gl_Position = vec4(a_position, 0, 1.0);\
    v_texCoords = a_texCoords;\
}\
";
static const char *fragment_shader = "\
#version 330 core\n   \
                      \
    in vec2 v_texCoords;  \
    \
    out vec4 f_color; \
    uniform sampler2D u_texture;\
                      \
    void main(){      \
        f_color = texture(u_texture, v_texCoords); }\
";
static Shader shader_planes;
static v_GLfloat_t plane_instances[MAX_TEXTURES] = {0};

static GLuint vao, vbo;
static v_GLuint_t textures_dirty;

void plane_renderer_init()
{
    shader_planes = shader_init_str(vertex_shader, fragment_shader);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // tex coords attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vec_new(GLuint, textures_dirty, MAX_TEXTURES);
}

static void add_vx(v_GLfloat_t *vx, float x0, float x1, float y0, float y1)
{
    vec_push(*vx, x0);
    vec_push(*vx, y0);
    vec_push(*vx, 0);
    vec_push(*vx, 0);

    vec_push(*vx, x0);
    vec_push(*vx, y1);
    vec_push(*vx, 0);
    vec_push(*vx, 1);

    vec_push(*vx, x1);
    vec_push(*vx, y0);
    vec_push(*vx, 1);
    vec_push(*vx, 0);

    vec_push(*vx, x0);
    vec_push(*vx, y1);
    vec_push(*vx, 0);
    vec_push(*vx, 1);

    vec_push(*vx, x1);
    vec_push(*vx, y1);
    vec_push(*vx, 1);
    vec_push(*vx, 1);

    vec_push(*vx, x1);
    vec_push(*vx, y0);
    vec_push(*vx, 1);
    vec_push(*vx, 0);
}

void plane_renderer_add(int x0, int y0, int x1, int y1, texture_t texture)
{
    if (texture >= MAX_TEXTURES)
    {
        warning("Too many textures registered, ignoring pending plane");
        return;
    }
    v_GLfloat_t *vertices = &plane_instances[texture];
    float x0f = 2.f * ((float)x0 / (float)g_scr.w) - 1.f;
    float x1f = 2.f * ((float)x1 / (float)g_scr.w) - 1.f;
    float y0f = 2.f * ((float)(g_scr.h - y0) / (float)g_scr.h) - 1.f;
    float y1f = 2.f * ((float)(g_scr.h - y1) / (float)g_scr.h) - 1.f;
    if (vertices->cap != 0)
    {
        add_vx(vertices, x0f, x1f, y0f, y1f);
        vec_push(textures_dirty, texture);
    }
    else
    {
        v_GLfloat_t vx;
        vec_new(GLfloat, vx, 24 * 16);
        add_vx(&vx, x0f, x1f, y0f, y1f);
        plane_instances[texture] = vx;
        vec_push(textures_dirty, texture);
    }
}

void plane_renderer_render()
{
    if (textures_dirty.size > 0)
    {
        shader_bind(shader_planes);
        glBindVertexArray(vao);

        for (int i = 0; i < textures_dirty.size; i++)
        {
            texture_t texture = vec_pop(textures_dirty);
            v_GLfloat_t *vertices = &plane_instances[texture];
            shader_bind_texture(shader_planes, texture, 0);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices->size * sizeof(*vertices->data), vertices->data, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, vertices->size / STRIDE);

            vertices->size = 0;
        }
        //glDisable(GL_BLEND);
    }
}

void plane_renderer_clean()
{
    for (int i = 0; i < MAX_TEXTURES; i++)
    {
        if (plane_instances[i].cap != 0)
            vec_free(plane_instances[i]);
    }
    vec_free(textures_dirty);
    shader_clean(shader_planes);
}