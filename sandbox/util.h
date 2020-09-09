//
// Created by uberdever on 06.09.2020.
//
#ifndef IND3D_UTIL_H
#define IND3D_UTIL_H

#include "alphabeta.h"

//Global typedefs
typedef struct Player {
    v2 pos;
    v2 dir, plane;
    float32 rotation_speed, angular_momentum;
    v2 speed, momentum;
} Player;

typedef struct Map
{
    int* data;
    int w, h;
    int tile_w, tile_h;
} Map;

typedef struct Projectile
{
    v2 pos, momentum;
    float32 radius, speed;
    bool is_alive;
} Projectile;

//Globals
extern Player player;
extern Map map;
extern Projectile* projectiles;

#endif