#include "model.h"

Model model_build(Mesh mesh)
{
    Model model = {};

    glGenVertexArrays(1, &model.info.vao);
    glBindVertexArray(model.info.vao);

    GLuint vbo_id = 0;

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size * sizeof(*mesh.vertices.data), mesh.vertices.data, GL_STATIC_DRAW);

    GLuint ibo_id = 0;

    glGenBuffers(1, &ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size * sizeof(*mesh.indices.data), mesh.indices.data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        model_vx_attr0, // positions
        GL_FLOAT,
        GL_FALSE,
        model_vx_count * sizeof(GLfloat),
        (GLvoid *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        model_vx_attr1, // texture coordinates
        GL_FLOAT,
        GL_FALSE,
        model_vx_count * sizeof(GLfloat),
        (GLvoid *)(model_vx_attr0 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        model_vx_attr2, // normals
        GL_FLOAT,
        GL_FALSE,
        model_vx_count * sizeof(GLfloat),
        (GLvoid *)((model_vx_attr0 + model_vx_attr1) * sizeof(GLfloat)));

    model.info.idx_count = mesh.indices.size;

    vec_new(Texture, model.textures, 2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return model;
}