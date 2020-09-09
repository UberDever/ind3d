//
// Created by uberdever on 06.09.2020.
//

#include "game.h"

static uint frames = 0;

void init(void) {
    map_load("../../data/map.map");
    vec_new(projectiles, 32);
    memset(projectiles, 0, vec_get_cap(projectiles) * sizeof(*projectiles));
    //SDL_ShowCursor(SDL_DISABLE);
}

void begin_frame(void) {
    g_screen_clear();
    g_screen_buffer_clear();
}

void end_frame(void) {
    g_screen_buffer_update();
    g_screen_present();
}

void event_handler(void)
{
    if (kbd_key_pressed(SDLK_w)) {
        player.momentum[1] = player.speed[1];
    }
    if (kbd_key_pressed(SDLK_s)) {
        player.momentum[1] = -player.speed[1];
    }
    if (kbd_key_pressed(SDLK_a)) {
        player.momentum[0] = -player.speed[0];
    }
    if (kbd_key_pressed(SDLK_d)) {
        player.momentum[0] = player.speed[0];
    }
    player.angular_momentum = -player.rotation_speed * mouse_get_rel_x();
    if (mouse_left_down())
    {
        projectile_create(player.pos, player.dir);
    }
    if (kbd_key_pushed(SDLK_c))
    {
        map.tile_w += 5;
        map.tile_h += 5;
    }
    if (kbd_key_pushed(SDLK_z))
    {
        map.tile_w -= 5;
        map.tile_h -= 5;
    }
}

void update(void) {
    player_update();
    projectile_update();
}

void render(void) {
    g_screen_draw_number(5, 5, frames, COLOR(yellow));
    frames++;
    if (frames > 60) frames = 0;

    const int half_w = (g_scr.w) / 2;
    const int half_h = (g_scr.h) / 2;
    //player_raycast();
    map_render((vi2) {half_w, half_h});
    player_render((vi2) {half_w, half_h});
    projectile_render((vi2) {half_w, half_h});
}

void clean(void) {
    vec_free(projectiles);
}