//
// Created by uberdever on 06.09.2020.
//

#include "map.h"

static void map_chunks_init(Map* map)
{
    map->chunks_w = map->w / C_CHUNK_W;
    map->chunks_h = map->h / C_CHUNK_H;
    vec_new(Chunk, map->chunks, map->chunks_w * map->chunks_h);
}

void map_init(Map *map, const char *path)
{
    map->tile_w = C_MAP_TILE_W;
    map->tile_h = C_MAP_TILE_H;

    FILE *map_source = NULL;
    map_source = fopen(path, "r");
    if (!map_source)
        error("Cannot load map, path: %s", path);

    fseek(map_source, 0, SEEK_END);
    uint map_len = ftell(map_source) + 1;
    fseek(map_source, 0, SEEK_SET);
    for (; 1; map->w++)
    {
        if (getc(map_source) == '\n' || map->w >= 1000)
            break;
    }
    fseek(map_source, 0, SEEK_SET);
    map->h = map_len / (map->w + 1);
    //debug("%d %d %d", map->w, map->h, map_len)
    ARR_ALLOC(map->data, map_len);
    for (uint i = 0; i < map->h; i++)
    {
        for (uint j = 0; j < map->w; j++)
        {
            map->data[i * map->w + j] = getc(map_source) - '0';
            //printf("%d", map->data[i * map->w + j]);
        }
        getc(map_source);
        //printf("\n");
    }

    map_chunks_init(map);
}

void map_render(Map* map, Player* player, v_Enemy_t *enemies, v_Projectile_t* projectiles, const vi2 pos, vi2 size)
{
    //Map
    vi2 offset, map_screen_start, map_screen_end, map_screen_size;
    offset[0] = map_screen_start[0] = pos[0] - (size[0] + .5f) * map->tile_w;
    offset[1] = map_screen_start[1] = pos[1] - (size[1] + .5f) * map->tile_h;
    v2 player_offset;
    pi_v2_copy(player->pos, player_offset);
    player_offset[0] -= (int)player_offset[0];
    player_offset[1] -= (int)player_offset[1];
    const v2 map_start = {player->pos[0] - size[0], player->pos[1] - size[1]};
    const int map_w = 2 * size[0] + 1, map_h = 2 * size[1] + 1;

    map_screen_size[0] = map_w * map->tile_w;
    map_screen_size[1] = map_h * map->tile_h;
    map_screen_end[0] = map_screen_start[0] + map_screen_size[0];
    map_screen_end[1] = map_screen_start[1] + map_screen_size[1];
    g_screen_fill_quat(VI2TOV2(offset), VI2TOV2(map_screen_size), COLOR(black));
    for (int i = 0; i < map_h; i++)
    {
        for (int j = 0; j < map_w; j++)
        {
            const v2 vx0 = {offset[0] + (j - player_offset[0] + .5) * map->tile_w,
                            offset[1] + (i - player_offset[1] + .5) * map->tile_h};
            const v2 vx1 = {vx0[0] + map->tile_w, vx0[1] + map->tile_h};
            const float32 map_y = (map_start[1] + i);
            const float32 map_x = map_start[0] + j;
            if (map_y >= 0 && map_x >= 0 && map_x < map->w && map_y < map->h)
            {
                if (tile_is_wall(map, map_x, map_y))
                {
                    g_screen_fill_quat(vx0, (v2){map->tile_w, map->tile_h}, COLOR(yellow));
                } /*else if (map->data[(int)map_y * map->w + (int)map_x] == 'E')
                {
                    g_screen_fill_quat(vx0, (v2){map->tile_w, map->tile_h}, COLOR(red));
                }*/
            }
            else
            {
                g_screen_draw_quat(vx0,
                                   (v2){vx0[0] + map->tile_w, vx0[1]},
                                   vx1,
                                   (v2){vx0[0], vx0[1] + map->tile_h}, COLOR(white));
            }
        }
    }
    //Player
    offset[0] = pos[0];
    offset[1] = pos[1];
    // g_screen_draw_line(offset[0],
    //                    offset[1],
    //                    offset[0] + player->dir[0] * (map->tile_w << 1),
    //                    offset[1] + player->dir[1] * (map->tile_h << 1), COLOR(red));
    g_screen_draw_line(offset[0],
                       offset[1],
                       offset[0] + player->right_frustum_ray[0] * (map->tile_w << 4),
                       offset[1] + player->right_frustum_ray[1] * (map->tile_h << 4), COLOR(cyan));
    g_screen_draw_line(offset[0],
                       offset[1],
                       offset[0] + player->left_frustum_ray[0] * (map->tile_w << 4),
                       offset[1] + player->left_frustum_ray[1] * (map->tile_h << 4), COLOR(cyan));
    g_screen_draw_circle(VI2TOV2(offset), player->hitbox_radius * map->tile_w, COLOR(green));

    v2 hitscan_left, hitscan_right, hitscan_left_long, hitscan_right_long;
    pi_v2_muls(player->plane, player->hitscan_range, hitscan_right);
    pi_v2_muls(hitscan_right, -1, hitscan_left);
    pi_v2_muls(player->plane, player->hitscan_range, hitscan_right_long);
    pi_v2_muls(hitscan_right_long, -1, hitscan_left_long);
    v2 tmp_dir;
    pi_v2_muls(player->dir, 10, tmp_dir);
    pi_v2_add(hitscan_left_long, tmp_dir, hitscan_left_long);
    pi_v2_add(hitscan_right_long, tmp_dir, hitscan_right_long);
    g_screen_draw_line(offset[0] + hitscan_left[0] * map->tile_w,
                       offset[1] + hitscan_left[1] * map->tile_h,
                       offset[0] + hitscan_left_long[0] * map->tile_w,
                       offset[1] + hitscan_left_long[1] * map->tile_h,
                       COLOR(magenta));
    g_screen_draw_line(offset[0] + hitscan_right[0] * map->tile_w,
                       offset[1] + hitscan_right[1] * map->tile_h,
                       offset[0] + hitscan_right_long[0] * map->tile_w,
                       offset[1] + hitscan_right_long[1] * map->tile_h,
                       COLOR(magenta));

    //Enemies
    offset[0] = pos[0] - player->pos[0] * map->tile_w;
    offset[1] = pos[1] - player->pos[1] * map->tile_h;
    for (uint i = 0; i < enemies->cap; i++)
    {
        if (enemies->data[i].is_alive)
        {
            const int rad = enemies->data[i].hitbox_radius * map->tile_w;
            v2 screen_pos = {offset[0] + enemies->data[i].pos[0] * map->tile_w, offset[1] + enemies->data[i].pos[1] * map->tile_h};
            //debug_v2(offset)
            //debug("%d %d", map_screen_pos[0], map_screen_pos[1])
            if ((int)screen_pos[0] - rad > map_screen_start[0] && (int)screen_pos[1] - rad > 0 &&
                (int)screen_pos[0] < (g_scr.w - rad) && (int)screen_pos[1] < (map_screen_end[1] - rad))
            {
                color enemy_color;
                const EnemyState state = (int)enemies->data[i].state->state;
                switch (state)
                {
                case EnemyState_Follow:
                    enemy_color = COLOR(orange);
                    break;

                case EnemyState_Attack:
                    enemy_color = COLOR(red);
                    break;

                default:
                    enemy_color = COLOR(light_gray);
                    break;
                }
                g_screen_draw_circle(screen_pos, rad, enemy_color);
            }
        }
    }
    //Projectiles
    offset[0] = pos[0] - player->pos[0] * map->tile_w;
    offset[1] = pos[1] - player->pos[1] * map->tile_h;
    for (uint i = 0; i < projectiles->cap; i++)
    {
        if (projectiles->data[i].is_alive)
        {
            const int rad = projectiles->data[i].radius * map->tile_w;
            v2 screen_pos = {offset[0] + projectiles->data[i].pos[0] * map->tile_w, offset[1] + projectiles->data[i].pos[1] * map->tile_h};
            if ((int)screen_pos[0] - rad > map_screen_start[0] && (int)screen_pos[1] - rad > 0 &&
                (int)screen_pos[0] < (g_scr.w - rad) && (int)screen_pos[1] < (map_screen_end[1] - rad))
                g_screen_draw_circle(screen_pos, rad, COLOR(blue));
        }
    }
    //Map frame
    g_screen_fill_quat((v2){g_scr.w - map_w * map->tile_w, 0},
                       (v2){map->tile_w, map_h * map->tile_h},
                       COLOR(dark_golden));
    g_screen_fill_quat((v2){g_scr.w - (map_w - 1) * map->tile_w, (map_h - 1) * map->tile_h},
                       (v2){map_w * map->tile_w, map->tile_h},
                       COLOR(dark_golden));

    // HUD
    color player_hp_color_start = COLOR(red);
    color player_hp_color_end = COLOR(green);
    color player_hp_color;
    player_hp_color.r = pi_lerp(player_hp_color_start.r, player_hp_color_end.r, MAX(player->hp / player->max_hp, 0));
    player_hp_color.g = pi_lerp(player_hp_color_start.g, player_hp_color_end.g, MAX(player->hp / player->max_hp, 0));
    player_hp_color.b = pi_lerp(player_hp_color_start.b, player_hp_color_end.b, MAX(player->hp / player->max_hp, 0));
    g_screen_draw_number(5, g_scr.h - 40, player->hp, player_hp_color);
    g_screen_draw_number(115, g_scr.h - 40, enemies->data[0].state_frame, COLOR(green));
}

void map_clean(Map* map) {
    free(map->data);
    vec_free(map->chunks);    
}
