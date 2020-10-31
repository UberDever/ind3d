#include "geometry.h"

// Vx returned in CCW direction
// 0--3
// | /|
// |/ |
// 1--2
void generate_quat(v_GLfloat_t *vertices, v_GLuint_t *indices, ChunkVx vx[4], GLuint vx_stride)
{
    GLuint start_index = vertices->size / vx_stride;

    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[0].p.v[i]);
    for (int i = 0; i < 2; i++)
        vec_push(*vertices, vx[0].t.v[i]);
    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[0].n.v[i]);

    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[1].p.v[i]);
    for (int i = 0; i < 2; i++)
        vec_push(*vertices, vx[1].t.v[i]);
    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[1].n.v[i]);

    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[2].p.v[i]);
    for (int i = 0; i < 2; i++)
        vec_push(*vertices, vx[2].t.v[i]);
    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[2].n.v[i]);

    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[3].p.v[i]);
    for (int i = 0; i < 2; i++)
        vec_push(*vertices, vx[3].t.v[i]);
    for (int i = 0; i < 3; i++)
        vec_push(*vertices, vx[3].n.v[i]);

    vec_push(*indices, start_index + 0);
    vec_push(*indices, start_index + 1);
    vec_push(*indices, start_index + 3);
    vec_push(*indices, start_index + 3);
    vec_push(*indices, start_index + 1);
    vec_push(*indices, start_index + 2);
}

void generate_pube(v_GLfloat_t *vertices, v_GLuint_t *indices, bool adj_check[4], int x, int y, v2_t t, GLuint cur_pube_idx)
{
    const GLfloat s = 0.5f;
    GLfloat vx[] = {
        -s, -s, -s, 1.f, 1.f, 0.f, 0.f, 0.f,
        -s, -s, +s, .25f, 1.f, 0.f, 0.f, 0.f,
        -s, +s, -s, 1.f, 0.f, 0.f, 0.f, 0.f,
        -s, +s, +s, .25f, 0.f, 0.f, 0.f, 0.f,
        +s, -s, -s, .75f, 1.f, 0.f, 0.f, 0.f,
        +s, -s, +s, .5f, 1.f, 0.f, 0.f, 0.f,
        +s, +s, -s, .75f, 0.f, 0.f, 0.f, 0.f,
        +s, +s, +s, .5f, 0.f, 0.f, 0.f, 0.f,
        -s, -s, -s, 0.f, 1.f, 0.f, 0.f, 0.f,
        -s, +s, -s, 0.f, 0.f, 0.f, 0.f, 0.f};
    for (uint i = 0; i < ARR_ONE_DIM_LEN(vx); i += 8)
    {
        vx[i + 0] += x;
        vx[i + 1] += 0;
        vx[i + 2] += y;
        vx[i + 3] += t.x;
        vx[i + 4] += t.y;
    }
    for (uint i = 0; i < ARR_ONE_DIM_LEN(vx); i++)
    {
        vec_push(*vertices, vx[i]);
    }
    GLuint cur_idx = cur_pube_idx * 10;
    if (adj_check[0])
    {
        GLuint ind[] = {0, 2, 4, 2, 6, 4};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
    if (adj_check[1])
    {
        GLuint ind[] = {6, 5, 4, 6, 7, 5};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
    if (adj_check[2])
    {
        GLuint ind[] = {7, 3, 5, 3, 1, 5};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
    if (adj_check[3])
    {
        GLuint ind[] = {3, 9, 1, 9, 8, 1};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
}
