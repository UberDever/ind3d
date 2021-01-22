#ifndef IND3D_VOXEL_SPRITE_H
#define IND3D_VOXEL_SPRITE_H

#include "graphics.h"
#include "shader.h"
#include "model.h"

// Voxel sprite is 3D model, made from sprite and, optionally, heightmap. Looks like minecraft tool without hegihtmap, but with it turns into stylized model
Model voxel_sprite_construct(const char *sprite_path, const char *depthmap_path, bool has_back, v3_t voxel_dimensions);

#endif