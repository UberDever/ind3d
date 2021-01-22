#include "line_renderer.h"

#define STRIDE 6

static const char *vertex_shader = "\
#version 330 core\n\
\
    layout(location = 0) in vec3 a_position;\
    layout(location = 1) in vec3 a_color;\
\
    out vec3 v_color;\
    \
void main()\
{\
    gl_Position = vec4(a_position, 1.0);\
    v_color = a_color;\
}\
";
static const char *fragment_shader = "\
#version 330 core\n   \
                      \
    out vec4 f_color; \
                      \
    in vec3 v_color;  \
                      \
    void main(){      \
        f_color = vec4(v_color, 1.0); }\
";
static Shader shader_lines;

static GLuint vao, vbo;
static v_GLfloat_t vertices;
static bool renderer_dirty = false;

void line_renderer_init()
{
    shader_lines = shader_init_str(vertex_shader, fragment_shader);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vec_new(GLfloat, vertices, 12 * 16);
}

void line_renderer_add_NDC(v3_t line_start, v3_t line_end, color color)
{
    const float r = (float)color.r / 0xff;
    const float g = (float)color.g / 0xff;
    const float b = (float)color.b / 0xff;
    vec_push(vertices, line_start.x);
    vec_push(vertices, line_start.y);
    vec_push(vertices, line_start.z);
    vec_push(vertices, r);
    vec_push(vertices, g);
    vec_push(vertices, b);
    vec_push(vertices, line_end.x);
    vec_push(vertices, line_end.y);
    vec_push(vertices, line_end.z);
    vec_push(vertices, r);
    vec_push(vertices, g);
    vec_push(vertices, b);
    renderer_dirty = true;
}

void line_renderer_add(int x0, int y0, int x1, int y1, color color)
{
    line_renderer_add_NDC((v3_t){2.f * ((float)x0 / (float)g_scr.w) - 1.f, 2.f * ((float)(g_scr.h - y0) / (float)g_scr.h) - 1.f, 0},
                          (v3_t){2.f * ((float)x1 / (float)g_scr.w) - 1.f, 2.f * ((float)(g_scr.h - y1) / (float)g_scr.h) - 1.f, 0},
                          color);
}

void line_renderer_render()
{
    glDisable(GL_DEPTH_TEST);
    shader_bind(shader_lines);

    glBindVertexArray(vao);
    if (renderer_dirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size * sizeof(*vertices.data), vertices.data, GL_DYNAMIC_DRAW);
        renderer_dirty = false;
    }
    glDrawArrays(GL_LINES, 0, vertices.size / STRIDE);

    vertices.size = 0;

    glEnable(GL_DEPTH_TEST);
}

void line_renderer_clean()
{
    vec_free(vertices);
    shader_clean(shader_lines);
}

void draw_cross(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x + width / 2, pos.y, pos.z}, (v3_t){pos.x + width / 2, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
}

void draw_one(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
}

void draw_two(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x, pos.y + height / 2, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x + width, pos.y, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y + height / 2, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
}

void draw_three(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x + width, pos.y, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
}

void draw_four(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
}

void draw_five(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x + width, pos.y, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
}

void draw_six(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x + width, pos.y, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
}

void draw_seven(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
}

void draw_eight(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x + width, pos.y, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
}

void draw_nine(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x + width, pos.y, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height / 2, pos.z}, (v3_t){pos.x + width, pos.y + height / 2, pos.z}, color);
}

void draw_zero(v3_t pos, float width, float height, color color)
{
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x + width, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y + height, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x + width, pos.y, pos.z}, color);
    line_renderer_add_NDC((v3_t){pos.x, pos.y, pos.z}, (v3_t){pos.x + width, pos.y + height, pos.z}, color);
}

void draw_symbol(char symbol, v3_t pos, float width, float height, color color)
{
    switch (symbol)
    {
    case '+':
        draw_cross(pos, width, height, color);
        break;
    case '1':
        draw_one(pos, width, height, color);
        break;

    case '2':
        draw_two(pos, width, height, color);
        break;

    case '3':
        draw_three(pos, width, height, color);
        break;

    case '4':
        draw_four(pos, width, height, color);
        break;

    case '5':
        draw_five(pos, width, height, color);
        break;

    case '6':
        draw_six(pos, width, height, color);
        break;

    case '7':
        draw_seven(pos, width, height, color);
        break;

    case '8':
        draw_eight(pos, width, height, color);
        break;

    case '9':
        draw_nine(pos, width, height, color);
        break;

    case '0':
        draw_zero(pos, width, height, color);
        break;

    default:
        break;
    }
}

void draw_number(int number, float offset, v3_t pos, float width, float height, color color)
{
    if (number == 0)
    {
        draw_zero(pos, width, height, color);
        return;
    }
    // if (number < 0)
    // {
    //     draw_minus(x, y, color);
    //     x += letter_offset;
    //     number *= -1;
    // }
    uint num = number;
    const uint digit_count = pi_log10(num);
    for (uint i = 0; num; i++)
    {
        v3_t num_pos = v3_add(pos, (v3_t){(float)(digit_count - i) * offset, 0, 0});
        draw_symbol('0' + num % 10, num_pos, width, height, color);
        num /= 10;
    }
}