//
// Created by uberdever on 09.09.2020.
//

#ifndef IND3D_ENEMY_H
#define IND3D_ENEMY_H

#include "../core.h"

void enemy_init(v_Enemy_t* enemies);

void enemy_create(v_Enemy_t *enemies, v2_t pos, EnemyTypes type);
void enemy_update(Map* map, Player* player, v_Enemy_t *enemies, v_Projectile_t* projectiles);

void enemies_process_hit(Map* map, Player* player, v_Enemy_t *enemies);

void enemy_clean(v_Enemy_t* enemies);

#endif //IND3D_ENEMY_H
