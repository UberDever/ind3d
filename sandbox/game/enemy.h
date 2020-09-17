//
// Created by uberdever on 09.09.2020.
//

#ifndef IND3D_ENEMY_H
#define IND3D_ENEMY_H

#include "../util.h"

void enemy_init();
void enemy_create(v2 pos, EnemyTypes type);
void enemy_update();
void enemy_render(vi2 offset);

#endif //IND3D_ENEMY_H
