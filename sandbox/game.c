//
// Created by uberdever on 06.09.2020.
//

#include "game.h"

static uint frames = 0;
int half_w;
int half_h;

void init(void) {

    half_w = (g_scr.w) / 2;
    half_h = (g_scr.h) / 2;

    map_load("./data/map.map");
    enemy_init();

    vec_new(projectiles, 32);
    memset(projectiles, 0, vec_get_cap(projectiles) * sizeof(*projectiles));
    vec_new(enemies, 32);
    memset(enemies, 0, vec_get_cap(enemies) * sizeof(*enemies));
    enemy_create((v2){6, 7}, Common);
    //enemy_create((v2){3, 6}, Common);
    //enemy_create((v2){10, 3.5}, Common);
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
#if 0
    float32 ro; v2 ro_dir; pi_v2_sub((v2){mouse_get_pos_x(), mouse_get_pos_y()},
                                     (v2){half_w, half_h},
                                     ro_dir);
    ro = pi_v2_len(ro_dir);
    float32 phi = last_phi;
    last_phi = atanf(ro_dir[1] / ro_dir[0]);
    float32 dphi = last_phi - phi;
#endif
    if (kbd_key_pressed(SDLK_w)) {
        player.momentum[1] = player.speed;
    }
    if (kbd_key_pressed(SDLK_s)) {
        player.momentum[1] = -player.speed;
    }
    if (kbd_key_pressed(SDLK_a)) {
        player.momentum[0] = player.speed;
    }
    if (kbd_key_pressed(SDLK_d)) {
        player.momentum[0] = -player.speed;
    }
    //player_event();
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
    enemy_update();
}

void render(void) {
    g_screen_draw_number(5, 5, frames, COLOR(yellow));
    frames++;
    if (frames > 60) frames = 0;

    const vi2 map_size = {10, 10};
    const vi2 player_screen_pos = {g_scr.w - (map_size[0] * map.tile_w), map_size[1] * map.tile_h};
    player_raycast();
    map_render(player_screen_pos, (vi2){10, 10});
}

void clean(void) {
    vec_free(projectiles);
    vec_free(enemies);
    vec_free(enemy_database);
}