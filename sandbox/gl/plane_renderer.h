#ifndef IND3D_PLANE_RENDERER_H
#define IND3D_PLANE_RENDERER_H

#include "../core.h"

void plane_renderer_init();
void plane_renderer_add(int x0, int y0, int x1, int y1, texture_t texture);
void plane_renderer_render();
void plane_renderer_clean();

#endif