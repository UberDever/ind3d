//
// Created by uberdever on 06.09.2020.
//
#ifndef IND3D_UTIL_H
#define IND3D_UTIL_H

#include "../include/alphabeta.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "gl/math_3d.h"

#include "config.h"

//Globals
extern int half_w, half_h;
extern uint frames;

extern Player player;
extern Map map;
extern v_Projectile_t projectiles;
extern v_Enemy_t enemies;
extern v_Enemyptr_t enemies_in_chunk;
extern v_Enemy_t enemy_database;
extern v_EnemyBehaviour_t enemy_behaviours;

static bool util_tile_is_wall(int x, int y)
{
    const int index = y * map.w + x;
    return map.data[index] != 0 &&
           map.data[index] != 'P' &&
           map.data[index] != 'E';
}

static void linecast(int start_x, int start_y, int end_x, int end_y, int* wall_x, int* wall_y, bool (*f)(int, int))
{
    int dx = abs(end_x - start_x), sx = start_x < end_x ? 1 : -1;
    int dy = -abs(end_y - start_y), sy = start_y < end_y ? 1 : -1;
    int err = dx+dy, e2; //error value e_xy
    while(1) { // loop
        if (start_x >= 0 && start_x < map.w && start_y >= 0 && start_y < map.h)
        {
            if (f(start_x, start_y))
            {
                *wall_x = start_x;
                *wall_y = start_y;
            }
        }
        e2 = 2*err;
        if (e2 >= dy) { // e_xy+e_x > 0
            if (start_x == end_x) break;
            err += dy; start_x += sx;
        }
        if (e2 <= dx) { // e_xy+e_y < 0
            if (start_y == end_y) break;
            err += dx; start_y += sy;
        }
    }
}

#endif