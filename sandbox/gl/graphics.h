
#ifndef IND3D_GRAPHICS_H
#define IND3D_GRAPHICS_H

#include "../include/alphabeta.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "stb_image.h"

// Common names for shader uniforms
static const char *u_model_name = "u_model";
static const char *u_view_name = "u_view";
static const char *u_projection_name = "u_projection";
static const char *u_iModel_name = "u_iModel";

//Generic vector registers
vec_register(GLfloat);
vec_register(GLuint);
vec_register(v3_t);
typedef v_v3_t_t vec_v3_t;

typedef v3_t* v3p_t;
vec_register(v3p_t);
typedef v_v3p_t_t vec_v3p_t;

typedef const char *pConstChar_t; // This should go to separate string class in alphabeta.h
vec_register(pConstChar_t);
typedef v_pConstChar_t_t v_pConstChar_t;

// 3d typedefs
typedef GLuint shader_t;

typedef struct Camera
{
    v3_t pos;
    v3_t dir;
    v3_t plane;
    v3_t up;
    m4_t view_matrix;
    m4_t projection_matrix;

    float32 sensitivity;

    float32 horisontal_rotation;
    float32 vertrical_rotation;
} Camera;

typedef GLuint texture_t;
typedef struct Texture
{
    texture_t id;
    int w, h;
} Texture;
vec_register(Texture);

typedef struct Mesh
{
    v_GLfloat_t vertices;
    v_GLuint_t indices;
} Mesh;

typedef struct BufferInfo
{
    GLuint vao;
    GLsizei idx_count;
} BufferInfo;

typedef struct Model
{
    BufferInfo info;
    v_Texture_t textures;
} Model;
vec_register(Model);

typedef struct ModelInstance
{
    Model model;
    m4_t transform;
} ModelInstance;
vec_register(ModelInstance);
#define MI(model_class, m4_transform) \
    (ModelInstance) { .transform = (m4_transform), .model = (model_class) }

static m4_t transform_instance(v3_t translation, v3_t rotation, v3_t scaling){
    m4_t tr_m4 = m4_translation(translation);
    m4_t ro_m4 = quaternion_rot_mat(quaternion_from_euler(rotation));
    m4_t sc_m4 = m4_scaling(scaling);
    return m4_mul(tr_m4, m4_mul(ro_m4, sc_m4));
}

mapl_register(v_ModelInstance_t, uint);
typedef struct ml_v_ModelInstance_tuint_t ml_VAO_to_Instance_t;

static inline Texture texture_load(char const *path)
{
    Texture texture = {};
    texture_t textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        error("Texture failed to load at path: %s", path);
        stbi_image_free(data);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    texture.id = textureID;
    texture.w = width;
    texture.h = height;
    return texture;
}

typedef struct Light
{
    v3_t ambient;
    v3_t diffuse;
    v3_t specular;
    v3_t color;
    float brightness;
} Light;

typedef struct DirLight
{
    Light properties;
    v3_t direction;
} DirLight;

typedef struct PointLight
{
    Light properties;
    v3_t position;
    v3_t attenuation;
} PointLight;

vec_register(DirLight);
vec_register(PointLight);

#endif