//
// Created by uberdever on 06.09.2020.
//

#include "map.h"

Map map = {.tile_w = 10, .tile_h = 10};

void map_load(const char *path) {
    FILE* map_source = NULL;
    map_source = fopen(path, "r");
    if (!map_source)
        error("Cannot load map, path: %s", path);

    fseek (map_source, 0, SEEK_END);
    uint map_len = ftell(map_source) + 1;
    fseek (map_source, 0, SEEK_SET);
    for (; 1; map.w++)
    {
        if (getc(map_source) == '\n' || map.w >= 1000)
            break;
    }
    fseek (map_source, 0, SEEK_SET);
    map.h = map_len / (map.w + 1);
    //debug("%d %d %d", map.w, map.h, map_len)
    ARR_ALLOC(map.data, map_len);
    for (uint i = 0; i < map.h; i++)
    {
        for (uint j = 0; j < map.w; j++)
        {
            map.data[i * map.w + j] = getc(map_source) - '0';
            //printf("%d", map.data[i * map.w + j]);
        }
        getc(map_source);
        //printf("\n");
    }
}

void map_render(const vi2 pos, vi2 size) {
    //Map
    vi2 offset;
    offset[0] = pos[0] - (size[0] + .5f) * map.tile_w;
    offset[1] = pos[1] - (size[1] + .5f) * map.tile_h;
    v2 player_offset; pi_v2_copy(player.pos, player_offset);
    player_offset[0] -= (int)player_offset[0]; player_offset[1] -= (int)player_offset[1];
    v2 map_start = {player.pos[0] - size[0], player.pos[1] - size[1]};
    const int map_w = 2 * size[0] + 1, map_h = 2 * size[1] + 1;
    g_screen_fill_quat((v2){offset[0], offset[1]}, (v2){map_w * map.tile_w, map_h * map.tile_h}, COLOR(black));
    for (int i = 0; i < map_h; i++)
    {
        for (int j = 0; j < map_w; j++)
        {
            const v2 vx0 = {offset[0] + (j - player_offset[0] + .5) * map.tile_w,
                            offset[1] + (i - player_offset[1] + .5) * map.tile_h};
            const v2 vx1 = {vx0[0] + map.tile_w, vx0[1] + map.tile_h};
            const float32 map_y = (map_start[1] + i);
            const float32 map_x = map_start[0] + j;
            if (map_y >= 0 && map_x >= 0 && map_x < map.w && map_y < map.h)
            {
                if (map.data[((int)map_y * map.w) + (int)map_x])
                {
                    g_screen_fill_quat(vx0, (v2){map.tile_w, map.tile_h}, COLOR(yellow));
                }
            }
            else
            {
                g_screen_draw_quat(vx0,
                                   (v2){vx0[0] + map.tile_w, vx0[1]},
                                   vx1,
                                   (v2){vx0[0], vx0[1] + map.tile_h}, COLOR(white));
            }
        }
    }
    //Player
    offset[0] = pos[0];
    offset[1] = pos[1];
    g_screen_draw_line(offset[0],
                       offset[1],
                       offset[0] + player.right_ray[0] * (map.tile_w << 4),
                       offset[1] + player.right_ray[1] * (map.tile_h << 4), COLOR(cyan));
    g_screen_draw_line(offset[0],
                       offset[1],
                       offset[0] + player.left_ray[0] * (map.tile_w << 4),
                       offset[1] + player.left_ray[1] * (map.tile_h << 4), COLOR(cyan));
    g_screen_draw_quat((v2){offset[0] - player.bounding_box[0] * map.tile_w, offset[1] - player.bounding_box[1] * map.tile_h},
                       (v2){offset[0] + player.bounding_box[0] * map.tile_w, offset[1] - player.bounding_box[1] * map.tile_h},
                       (v2){offset[0] + player.bounding_box[0] * map.tile_w, offset[1] + player.bounding_box[1] * map.tile_h},
                       (v2){offset[0] - player.bounding_box[0] * map.tile_w, offset[1] + player.bounding_box[1] * map.tile_h},
                       COLOR(green));
    //Enemies
    offset[0] = pos[0] - player.pos[0] * map.tile_w;
    offset[1] = pos[1] - player.pos[1] * map.tile_h;
    for (uint i = 0; i < vec_get_cap(enemies); i++)
    {
        if (enemies[i].is_alive)
        {
            const int rad = enemies[i].hitbox_radius * map.tile_w;
            v2 screen_pos = { offset[0] + enemies[i].pos[0] * map.tile_w, offset[1] + enemies[i].pos[1] * map.tile_h};
            if (screen_pos[0] > rad && screen_pos[1] > rad &&
                screen_pos[0] < (g_scr.w - rad) && screen_pos[1] < (g_scr.h - rad))
                g_screen_draw_circle(screen_pos, rad, enemies[i].is_active ? COLOR(red) : COLOR(light_gray));
        }
    }
    //Projectiles
    offset[0] = pos[0] - player.pos[0] * map.tile_w;
    offset[1] = pos[1] - player.pos[1] * map.tile_h;
    for (uint i = 0; i < vec_get_cap(projectiles); i++)
    {
        if (projectiles[i].is_alive)
        {
            const int rad = projectiles[i].radius * map.tile_w;
            v2 screen_pos = { offset[0] + projectiles[i].pos[0] * map.tile_w, offset[1] + projectiles[i].pos[1] * map.tile_h};
            if (screen_pos[0] > rad && screen_pos[1] > rad &&
                screen_pos[0] < (g_scr.w - rad) && screen_pos[1] < (g_scr.h - rad))
                g_screen_draw_circle(screen_pos, rad, COLOR(blue));
        }
    }
    //Map frame
    g_screen_fill_quat( (v2){g_scr.w - map_w * map.tile_w, 0},
                        (v2){map.tile_w, map_h * map.tile_h},
                        COLOR(cyan));
    g_screen_fill_quat((v2){g_scr.w - (map_w - 1) * map.tile_w, (map_h - 1) * map.tile_h},
                       (v2){map_w * map.tile_w, map.tile_h},
                       COLOR(cyan));
}
