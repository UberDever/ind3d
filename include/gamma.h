//
// Created by uberdever on 31.07.2020.
//
// Gamma is graphics utility header, pure functions for abstraction from platform
#ifndef ALPHABETA_GAMMA_H
#define ALPHABETA_GAMMA_H

#include "alpha.h"
#include "pi.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

extern struct scr_t {
    uint w, h, pixel_count;
    uint32* screen_buf;
    bool is_fullscreen;
} g_scr;

void g_init(uint w, uint h, bool is_fullscreen, SDL_WindowFlags win_flags, SDL_RendererFlags ren_flags);
void g_clean();

void g_screen_set_render_color(color color);

void g_screen_clear();
void g_screen_fill(color color);
void g_screen_put_pixel(uint x, uint y, color color);
void g_screen_draw_line(int start_x, int start_y, int end_x, int end_y, color color);
void g_screen_draw_tri(const v2 vx0, const v2 vx1, const v2 vx2, color color);
void g_screen_draw_quat(const v2 vx0, const v2 vx1, const v2 vx2, const v2 vx3, color color);

void g_screen_fill_quat(const v2 pos, const v2 size, color color);

void g_screen_draw_symbol(uint x, uint y, uint8 symbol, color color);
void g_screen_draw_number(uint x, uint y, uint number, color color);
void g_screen_draw_string(uint x, uint y, const char* string, color color);

void g_screen_buffer_update();
void g_screen_buffer_clear();

void g_screen_present();

static void circle_eight_pixels(int xc, int yc, int x, int y, color color)
{
    g_screen_put_pixel(x + xc,   y + yc, color);
    g_screen_put_pixel(x + xc,  -y + yc, color);
    g_screen_put_pixel(-x + xc, -y + yc, color);
    g_screen_put_pixel(-x + xc,  y + yc, color);
    g_screen_put_pixel(y + xc,   x + yc, color);
    g_screen_put_pixel(y + xc,  -x + yc, color);
    g_screen_put_pixel(-y + xc, -x + yc, color);
    g_screen_put_pixel(-y + xc,  x + yc, color);
}

static void pi_screen_draw_circle(v2 center, int r, color color)
{
    int x = 0, y = r, d = 3 - (2 * r);
    circle_eight_pixels(center[0], center[1], x, y, color);

    while(x <= y)
    {
        if(d <= 0)
        {
            d += (4 * x) + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
        circle_eight_pixels(center[0], center[1], x, y, color);
    }
}

#endif //ALPHABETA_GAMMA_H
