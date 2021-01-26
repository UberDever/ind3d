#ifndef IND3D_LINE_RENDERER_H
#define IND3D_LINE_RENDERER_H

#include "../core.h"
#include "shader.h"

void line_renderer_init();
void line_renderer_add(int x0, int y0, int x1, int y1, color color);
void line_renderer_add_NDC(v3_t line_start, v3_t line_end, color color);
void line_renderer_add_quat(int x0, int y0, int w, int h, color color);
void line_renderer_add_circle(int x0, int y0, int radius, color color);
void line_renderer_render();
void line_renderer_clean();

void draw_symbol(char symbol, v3_t pos, float width, float height, color color);
void draw_number(int number, float offset, v3_t pos, float width, float height, color color);

#endif