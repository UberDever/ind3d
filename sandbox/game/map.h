//
// Created by uberdever on 06.09.2020.
//

#ifndef IND3D_MAP_H
#define IND3D_MAP_H

#include "../core.h"

void map_init(Map* map, const char* path, const char* tileset_path, const char* vertex_shader, const char* fragment_shader);

void map_render(Map *map, Player *player, v_Enemy_t *enemies, v_Projectile_t *projectiles, const int pos_x, const int pos_y, const int size_x, const int size_y);

void map_clean(Map* map);

#endif //IND3D_MAP_H
