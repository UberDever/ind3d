//
// Created by uberdever on 06.09.2020.
//

#ifndef IND3D_GAME_H
#define IND3D_GAME_H

#include "util.h"

#include "game/map.h"
#include "game/player.h"
#include "game/projectile.h"
#include "game/enemy.h"

void init(void);
void event_handler(void);
void begin_frame(void);
void update(void);
void render(void);
void end_frame(void);
void clean(void);

#endif //IND3D_GAME_H
