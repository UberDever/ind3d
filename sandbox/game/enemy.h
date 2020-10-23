//
// Created by uberdever on 09.09.2020.
//

#ifndef IND3D_ENEMY_H
#define IND3D_ENEMY_H

#include "enemy_states.h"

void enemy_init();
void enemy_create(v2 pos, EnemyTypes type);
void enemy_update();
void enemy_render(vi2 offset);

void enemies_process_hit();
#endif //IND3D_ENEMY_H
