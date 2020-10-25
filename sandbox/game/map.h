//
// Created by uberdever on 06.09.2020.
//

#ifndef IND3D_MAP_H
#define IND3D_MAP_H

#include "../core.h"

void map_init(Map* map, const char* path);

void map_render(Map* map, Player* player, v_Enemy_t *enemies, v_Projectile_t* projectiles, const vi2 pos, vi2 size);

void map_clean(Map* map);

#endif //IND3D_MAP_H
