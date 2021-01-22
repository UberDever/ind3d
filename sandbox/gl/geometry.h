#ifndef IND3D_GEOMETRY_H
#define IND3D_GEOMETRY_H

#include "graphics.h"

#define tileset_w 16
#define tileset_h 16
#define tileset_tile_w 64
#define tileset_tile_h 64
#define tileset_tex_w 1024
#define tileset_tex_h 1024
void generate_pube(v_GLfloat_t *vertices, v_GLuint_t *indices, bool sides[4], int x, int y, v2_t t, GLuint cur_pube_idx);

Mesh obj_parse(const char *path);
Mesh generate_pyramid(const GLfloat base, const GLfloat height);
Mesh generate_sphere(const GLuint num_slices, const GLfloat radius);
Mesh generate_plane(v3_t p0, v3_t p1, v3_t p2, v3_t p3);
Mesh generate_hyperboloid(float32 height, float32 a, float32 b, uint offset, uint num_vertices);

#endif
