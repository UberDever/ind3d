//
// Created by uberdever on 06.09.2020.
//

#include "game.h"

uint frames = 0;
int half_w;
int half_h;

void init(void)
{
    half_w = (g_scr.w) / 2;
    half_h = (g_scr.h) / 2;

    map_init("../../data/huge.map");
    player_init();
    enemy_init();
    projectile_init();

    enemy_create((v2){6, 7}, EnemyTypes_Common);
    //enemy_create((v2){3, 12}, EnemyTypes_Strong);
    //enemy_create((v2){10, 3.5}, EnemyTypes_Boss);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_CaptureMouse(SDL_TRUE);

}

void begin_frame(void)
{
    g_screen_clear();
    g_screen_buffer_clear();
}

void end_frame(void)
{
    g_screen_buffer_update();
    g_screen_present();
}

void event_handler(void)
{
    if (kbd_key_pressed(SDLK_ESCAPE))
        is_program_running = false;
#if 0
    float32 ro; v2 ro_dir; pi_v2_sub((v2){mouse_get_pos_x(), mouse_get_pos_y()},
                                     (v2){half_w, half_h},
                                     ro_dir);
    ro = pi_v2_len(ro_dir);
    float32 phi = last_phi;
    last_phi = atanf(ro_dir[1] / ro_dir[0]);
    float32 dphi = last_phi - phi;
#endif
    if (kbd_key_pressed(SDLK_w))
    {
        player.momentum[1] = player.speed;
    }
    if (kbd_key_pressed(SDLK_s))
    {
        player.momentum[1] = -player.speed;
    }
    if (kbd_key_pressed(SDLK_a))
    {
        player.momentum[0] = player.speed;
    }
    if (kbd_key_pressed(SDLK_d))
    {
        player.momentum[0] = -player.speed;
    }
    player.angular_momentum = -player.rotation_speed * mouse_get_rel_x();
    //player_event();
    //enemy_process_hit();
    if (mouse_left_down())
    {
        enemies_process_hit();
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

void update(void)
{
    for (int x = 0; x < map.w; x++)
    {
        for (int y = 0; y < map.h; y++)
        {
            if (!util_tile_is_wall(x, y))
                map.data[y * map.w + x] = 0;
        }
    }
    player_update();
    projectile_update();
    enemy_update();
}

void render(void)
{
    vi2 map_size = {16, 16};
    const vi2 player_screen_pos = {g_scr.w - (map_size[0] * map.tile_w), map_size[1] * map.tile_h};
    player_raycast();
    map_render(player_screen_pos, map_size);

    g_screen_draw_number(5, 5, frames, COLOR(yellow));
    frames++;
    if (frames >= 60)
        frames = 0;
}

void clean(void)
{
    vec_free(projectiles);
    vec_free(enemies);
    vec_free(enemy_database);
}