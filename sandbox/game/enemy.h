//
// Created by uberdever on 09.09.2020.
//

#ifndef IND3D_ENEMY_H
#define IND3D_ENEMY_H

#include "../core.h"

void enemies_init(v_Enemy_t* enemies, Map* map);

void enemies_create(v_Enemy_t *enemies, v2_t pos, EnemyTypes type);
void enemies_update(Map* map, Player* player, v_Enemy_t *enemies, v_Projectile_t* projectiles);
void enemies_render(v_Enemy_t *enemies);

void enemies_process_hit(Map* map, Player* player, v_Enemy_t *enemies);

void enemies_clean(v_Enemy_t* enemies);

#endif //IND3D_ENEMY_H
