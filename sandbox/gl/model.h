#ifndef IND3D_MODEL_H
#define IND3D_MODEL_H

#include "geometry.h"
#include "shader.h"

#define model_vx_attr0 3
#define model_vx_attr1 2
#define model_vx_attr2 3
#define model_vx_count (model_vx_attr0 + model_vx_attr1 + model_vx_attr2)

Model model_build(Mesh mesh);

static void model_add_texture(Model* model, Texture texture)
{
    vec_push(model->textures, texture);
}

static void model_clean(Model model)
{
    vec_free(model.textures);
}


#endif