//
// Created by uberdever on 06.09.2020.
//

#ifndef IND3D_PLAYER_H
#define IND3D_PLAYER_H

#include "../core.h"

void player_init(Player* player);

void player_event(Player* player);
void player_update(Map* map, Player* player);
void player_render(Player *player);

void player_raycast(Map* map, Player* player);

void player_clean(Player* player);

#endif //IND3D_PLAYER_H
