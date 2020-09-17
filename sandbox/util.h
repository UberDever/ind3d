//
// Created by uberdever on 06.09.2020.
//
#ifndef IND3D_UTIL_H
#define IND3D_UTIL_H

#include "alphabeta.h"

//Global typedefs
typedef struct Player {
    v2 pos, momentum;
    v2 dir, plane;
    v2 left_ray, right_ray, bounding_box;
    float32 speed, rotation_speed, angular_momentum, dir_scale, plane_scale;
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

typedef enum EnemyTypes {
    Void = 0,
    Common = 1,
    Strong = 2,
    Boss = 3
} EnemyTypes;

typedef struct Enemy {
    v2 pos, momentum;
    float32 hitbox_radius, speed;
    enum EnemyTypes type;
    bool is_active, is_alive;
} Enemy;

//Globals
extern int half_w, half_h;

extern Player player;
extern Map map;
extern Projectile* projectiles;
extern Enemy* enemies;
extern Enemy* enemy_database;

#endif