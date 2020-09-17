//
// Created by uberdever on 31.07.2020.
//
#include "../include/gamma.h"

struct scr_t g_scr;

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* screen;
static color renderer_color = COLOR(black);

void g_init(uint w, uint h, bool is_fullscreen, SDL_WindowFlags win_flags, SDL_RendererFlags ren_flags) {
    int exitcode = 0;
    g_scr.w = w; g_scr.h = h; g_scr.is_fullscreen = is_fullscreen;
    win_flags |= g_scr.is_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    if ((exitcode = SDL_Init(SDL_INIT_EVERYTHING)) < 0)
        error("SDL couldn't init properly, error: %d", exitcode);

    if ((exitcode = TTF_Init()) == -1)
        error("SDL_ttf couldn't init properly, error: %d", exitcode);

    if ((exitcode = IMG_Init(IMG_INIT_PNG)) != IMG_INIT_PNG)
        error("SDL_image couldn't init properly, error: %d", exitcode);

    window = SDL_CreateWindow("Sandbox", 20, 20, g_scr.w, g_scr.h, win_flags);

    if (!window)
        error("Couldn't create SDL window")

    renderer = SDL_CreateRenderer(window, -1, ren_flags);
    if (!renderer)
        error("Couldn't create SDL renderer")

    screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, g_scr.w, g_scr.h);
    g_scr.pixel_count = g_scr.w * g_scr.h;
    ARR_ALLOC(g_scr.screen_buf, g_scr.pixel_count);
    ALLOC_CHECK(g_scr.screen_buf);
    g_screen_buffer_clear();
    SDL_SetTextureBlendMode(screen, SDL_BLENDMODE_BLEND);
}

void g_clean() {
    free(g_scr.screen_buf);
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void g_screen_set_render_color(color color) {
    renderer_color = color;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void g_screen_put_pixel(uint x, uint y, color color) {
    g_scr.screen_buf[y * g_scr.w + x] = color.as_uint;
}

void g_screen_clear() {
    g_screen_set_render_color(renderer_color);
    SDL_RenderClear(renderer);
}

void g_screen_fill(color color) {
    g_screen_set_render_color(color);
    SDL_RenderClear(renderer);
}

void g_screen_draw_line(int start_x, int start_y, int end_x, int end_y, color color)
{
    int dx = abs(end_x - start_x), sx = start_x < end_x ? 1 : -1;
    int dy = -abs(end_y - start_y), sy = start_y < end_y ? 1 : -1;
    int err = dx+dy, e2; //error value e_xy
    while(1) { // loop
        if (start_x >= 0 && start_x < g_scr.w && start_y >= 0 && start_y < g_scr.h)
        {
            g_scr.screen_buf[(start_y * g_scr.w) + start_x] = color.as_uint;
        }
        e2 = 2*err;
        if (e2 >= dy) { // e_xy+e_x > 0
            if (start_x == end_x) break;
            err += dy; start_x += sx;
        }
        if (e2 <= dx) { // e_xy+e_y < 0
            if (start_y == end_y) break;
            err += dx; start_y += sy;
        }
    }
}

void g_screen_draw_tri(const v2 vx0, const v2 vx1, const v2 vx2, color color) {
    g_screen_draw_line(vx0[0], vx0[1], vx1[0], vx1[1], color);
    g_screen_draw_line(vx1[0], vx1[1], vx2[0], vx2[1], color);
    g_screen_draw_line(vx2[0], vx2[1], vx0[0], vx0[1], color);
}

void g_screen_draw_quat(const v2 vx0, const v2 vx1, const v2 vx2, const v2 vx3, color color) {
    g_screen_draw_line(vx0[0], vx0[1], vx1[0], vx1[1], color);
    g_screen_draw_line(vx1[0], vx1[1], vx2[0], vx2[1], color);
    g_screen_draw_line(vx2[0], vx2[1], vx3[0], vx3[1], color);
    g_screen_draw_line(vx3[0], vx3[1], vx0[0], vx0[1], color);
}

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

void g_screen_draw_circle(v2 center, int r, color color)
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

void g_screen_fill_quat(const v2 pos, const v2 size, color color) {
    for (int i = 0; i < size[0]; i++)
        for (int j = 0; j < size[1]; j++)
        {
            const int y = (j + (int)pos[1]);
            const int x = (i + (int)pos[0]);
            if (x >= 0 && x < g_scr.w && y >= 0 && y < g_scr.h)
                g_scr.screen_buf[y * g_scr.w + x] = color.as_uint;
        }

}

void g_screen_buffer_update() {
    SDL_UpdateTexture(screen, 0, g_scr.screen_buf, sizeof(uint32) * g_scr.w);
    SDL_RenderCopy(renderer, screen, 0, 0);
}

void g_screen_buffer_clear() {
    memset(g_scr.screen_buf, 0x0, sizeof(uint32) * g_scr.w * g_scr.h);
}

void g_screen_present() {
    SDL_RenderPresent(renderer);
}


