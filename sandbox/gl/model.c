#include "model.h"

static void gen_vao(Model *model)
{
    if (model->info.vao != 0)
        model_clear(model);

    glGenVertexArrays(1, &model->info.vao);
    glBindVertexArray(model->info.vao);
}

static void add_vbo(Model *model, GLuint dimensions, GLfloat *data, uint data_count)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data_count * sizeof(*data), data,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(model->buffers.size, dimensions, GL_FLOAT,
                          GL_FALSE, 0, (GLvoid *)0);

    glEnableVertexAttribArray(model->buffers.size);

    vec_push(model->buffers, vbo);
}

static void add_ibo(Model *model, GLuint *data, uint data_count)
{
    model->info.idx_count = data_count;
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_count * sizeof(*data),
                 data, GL_STATIC_DRAW);
}

void model_generate(Model *model, Mesh *mesh)
{
    gen_vao(model);

    vec_new(GLuint, model->buffers, 4);

    add_vbo(model, 3, mesh->positions.data, mesh->positions.size);
    add_vbo(model, 2, mesh->texture_coordinates.data, mesh->texture_coordinates.size);
    add_ibo(model, mesh->indices.data, mesh->indices.size);
}

ModelInfo model_get_info(Model *model) {
    return model->info;
}

void model_clear(Model *model)
{
    if (model->info.vao)
        glDeleteVertexArrays(1, &model->info.vao);
    if (model->buffers.size > 0)
        glDeleteBuffers(model->buffers.size,
                        model->buffers.data);

    model->buffers.size = 0;
    model_info_reset(&model->info);
}