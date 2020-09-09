//
// Created by uberdever on 06.09.2020.
//

#include "player.h"

static const v2 bounding_box = {0.5, 0.5};

Player player = {.pos = {2, 2}, .dir = {0, 1}, .plane = {-1, 0},
                 .rotation_speed = DEG2RAD(3), .angular_momentum = 0,
                 .speed = {0.1, 0.1}, .momentum = {}};

void player_update() {
    pi_v2_rotate(player.dir, player.angular_momentum, player.dir);
    pi_v2_rotate(player.plane, player.angular_momentum, player.plane);
    player.angular_momentum = 0;

    v2 tmp0 = {}; pi_v2_muls(player.dir, player.momentum[1], tmp0);
    v2 tmp1 = {}; pi_v2_muls(player.plane, player.momentum[0], tmp1);
    v2 step = {}; pi_v2_add(tmp0, tmp1, step);

    v2 p0; p0[0] = player.pos[0] - bounding_box[0]; p0[1] = player.pos[1] - bounding_box[1];
    v2 p1; p1[0] = player.pos[0] + bounding_box[0]; p1[1] = player.pos[1] - bounding_box[1];
    v2 p2; p2[0] = player.pos[0] + bounding_box[0]; p2[1] = player.pos[1] + bounding_box[1];
    v2 p3; p3[0] = player.pos[0] - bounding_box[0]; p3[1] = player.pos[1] + bounding_box[1];
    if (!(  map.data[(int)p0[1] * map.w + (int)(p0[0] + step[0])] ||
            map.data[(int)p1[1] * map.w + (int)(p1[0] + step[0])] ||
            map.data[(int)p2[1] * map.w + (int)(p2[0] + step[0])] ||
            map.data[(int)p3[1] * map.w + (int)(p3[0] + step[0])] )) { player.pos[0] += step[0]; }
    if (!(  map.data[(int)(p0[1] + step[1]) * map.w + (int)p0[0]] ||
            map.data[(int)(p1[1] + step[1]) * map.w + (int)p1[0]] ||
            map.data[(int)(p2[1] + step[1]) * map.w + (int)p2[0]] ||
            map.data[(int)(p3[1] + step[1]) * map.w + (int)p3[0]] )) { player.pos[1] += step[1]; }
    player.momentum[0] = 0;
    player.momentum[1] = 0;
}

void player_render(vi2 pos) {
    g_screen_draw_line(pos[0],
                       pos[1],
                       pos[0] + player.dir[0] * map.tile_w,
                       pos[1] + player.dir[1] * map.tile_h,
                       COLOR(red));
    const int half_w = (g_scr.w) / 2;
    const int half_h = (g_scr.h) / 2;
    g_screen_draw_quat((v2){half_w - bounding_box[0] * map.tile_w, half_h - bounding_box[1] * map.tile_h},
                       (v2){half_w + bounding_box[0] * map.tile_w, half_h - bounding_box[1] * map.tile_h},
                       (v2){half_w + bounding_box[0] * map.tile_w, half_h + bounding_box[1] * map.tile_h},
                       (v2){half_w - bounding_box[0] * map.tile_w, half_h + bounding_box[1] * map.tile_h},
                       COLOR(green));
}

void player_raycast() { //TODO: REBUILD
    int scrW = g_scr.w;
    int scrH = g_scr.h;

    int dS = 0;
    int dE = 0;

    g_screen_fill(COLOR(black));

    for (int x = 0; x < scrW; ++x) {
        float32 cam = 2.f * (float)x / (float)scrW - 1;
        v2 ray; pi_v2_copy(player.plane, ray);
        pi_v2_muls(ray, cam, ray);
        pi_v2_add(ray, player.dir, ray);
        vi2 pos = {player.pos[0], player.pos[1]};
        v2 side;
        v2 delta = {fabsf(1.f / ray[0]),fabsf(1.f / ray[1])};
        float32 pwd = 0;
        vi2 step;
        bool hit = false, NS = true; //North-South

        if (ray[0] > 0)
        {
            step[0] = 1;
            side[0] = ((float32)pos[0] + 1 - player.pos[0]) * delta[0];
        } else
        {
            step[0] = -1;
            side[0] = (player.pos[0] - pos[0]) * delta[0];
        }
        if (ray[1] > 0)
        {
            step[1] = 1;
            side[1] = ((float32)pos[1] + 1 - player.pos[1]) * delta[1];
        } else
        {
            step[1] = -1;
            side[1] = (player.pos[1] - pos[1]) * delta[1];
        }
        while (!hit)
        {
            if (side[0] < side[1])
            {
                side[0] += delta[0];
                pos[0] += step[0];
                NS = true;
            } else
            {
                side[1] += delta[1];
                pos[1] += step[1];
                NS = false;
            }
            if (map.data[pos[1] * map.w + pos[0]]) hit = true;
        }

        if (NS) pwd = (pos[0] - player.pos[0] + (1.f - step[0]) / 2) / ray[0];
        else    pwd = (pos[1] - player.pos[1] + (1.f - step[1]) / 2) / ray[1];

        int line = (int)(2 * (float32)scrH / pwd);
        if (line) //for evading zero-length lines
        {
            dS = -line / 2 + (scrH) / 2; dS = dS < 0 ? 0 : dS;
            dE = line / 2 + (scrH) / 2; dE = dE >= scrH ? scrH - 1 : dE;
            //dS = -line / 2 + scrH / 2 - pd->pl->offset; dS = dS < 0 ? 0 : dS;
            //dE = line / 2 + scrH / 2 - pd->pl->offset; dE = dE >= scrH ? scrH - 1 : dE;
        }

        g_screen_draw_line(x, dS, x, dE, COLOR(cyan));
    }
    //SDL_Rect o = {0, 0, g_scr.w, g_scr.h};
    //SDL_UpdateTexture(global::gameScreen(), 0, global::gameScreenBuffer(), sizeof(uint32_t) * global::cfg::scrRenW());
    ////SDL_RenderCopy(global::renderer(), skybox, 0, &o);
    //SDL_RenderCopy(global::renderer(), global::gameScreen(), 0, 0);
    //SDL_RenderPresent(global::renderer());
}