#ifndef IND3D_ENEMY_STATES_H
#define IND3D_ENEMY_STATES_H

#include "../core.h"

extern v_Enemy_t enemy_database;
extern v_EnemyBehaviour_t enemy_behaviours;

void state_idle(Map* map, Player* player, Enemy *enemy);
void state_follow(Map* map, Player* player, Enemy *enemy);
void state_attack(Map* map, Player* player, Enemy *enemy);

#endif