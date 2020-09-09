//
// Created by uberdever on 06.09.2020.
//

#include "map.h"

Map map = {.tile_w = 32, .tile_h = 32};

void map_load(const char *path) {
    FILE* map_source = NULL;
    map_source = fopen(path, "r");
    if (!map_source)
        error("Cannot load map, path: %s", path);

    fseek (map_source, 0, SEEK_END);
    uint map_len = ftell(map_source);
    fseek (map_source, 0, SEEK_SET);
    for (; 1; map.w++)
    {
        if (getc(map_source) == '\n' || map.w >= 1000)
            break;
    }
    fseek (map_source, 0, SEEK_SET);
    map.h = map_len / map.w;
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

void map_render(vi2 offset) {
    offset[0] -= player.pos[0] * map.tile_w; offset[1] -= player.pos[1] * map.tile_h;
    for (int i = 0; i < map.h; i++)
    {
        for (int j = 0; j < map.w; j++)
        {
            const v2 vx0 = {offset[0] + j * map.tile_w, offset[1] + i * map.tile_h};
            const v2 vx1 = {vx0[0] + map.tile_w, vx0[1] + map.tile_h};
            if ((vx0[0] < g_scr.w && vx0[1] < g_scr.h) &&
                (vx1[0] >= 0 && vx1[1] >= 0))
            {
                if (map.data[i * map.w + j])
                {
                    g_screen_fill_quat(vx0, (v2){map.tile_w, map.tile_h}, COLOR(yellow));
                }
                else
                {
                    //g_screen_draw_quat(vx0,
                    //                   (v2){vx0[0] + map.tile_w, vx0[1]},
                    //                   vx1,
                    //                   (v2){vx0[0], vx0[1] + map.tile_h}, COLOR(white));
                }
            }
        }
    }
}
