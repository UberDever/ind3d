#ifndef IND3D_MODEL_H
#define IND3D_MODEL_H

#include "graphics.h"

typedef struct ModelInfo
{
    GLuint vao;
    GLsizei idx_count;
    GLenum draw_mode;
} ModelInfo;

static void model_info_reset(ModelInfo *info)
{
    info->vao = 0;
    info->idx_count = 0;
    info->draw_mode = 0;
}

typedef struct Mesh
{
    v_GLfloat_t positions;
    v_GLfloat_t texture_coordinates;
    v_GLuint_t indices;
} Mesh;

typedef struct Model
{
    ModelInfo info;
    v_GLuint_t buffers;
} Model;

void model_generate(Model *model, Mesh *mesh);
void model_clear(Model *model);
ModelInfo model_get_info(Model *model);

typedef struct ChunkVx
{
    v3_t p;
    v2_t t;
    v3_t n;
} ChunkVx;

#endif