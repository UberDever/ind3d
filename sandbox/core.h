//
// Created by uberdever on 06.09.2020.
//
#ifndef IND3D_CORE_H
#define IND3D_CORE_H

#include "gl/math_3d.h"
#include "gl/model.h"

#include "config.h"

//Global typedefs
typedef struct Player
{
    v2 pos, momentum;
    v2 dir, plane;
    v2 left_frustum_ray, right_frustum_ray;
    float32 angular_momentum, dir_scale, plane_scale, shoot_scale, hitbox_radius;
    // Game variables
    float32 speed, rotation_speed;
    float32 hp, max_hp;
    float32 hitscan_range;
} Player;

typedef struct Chunk
{

} Chunk;
vec_register(Chunk);

typedef struct Map
{
    int *data;
    int w, h;
    int tile_w, tile_h;
    int chunks_w, chunks_h;
    v_Chunk_t chunks;
} Map;

typedef struct Projectile
{
    v2 pos, momentum;
    float32 radius, speed;
    bool is_alive;
} Projectile;
vec_register(Projectile);

typedef enum EnemyTypes
{
    EnemyTypes_Void = 0,
    EnemyTypes_Common = 1,
    EnemyTypes_Strong = 2,
    EnemyTypes_Boss = 3,
    EnemyTypes_Common_Range = 4,
    EnemyTypes_Strong_Range = 5,
    EnemyTypes_Boss_Range = 6,
    EnemyTypes_Size = 7
} EnemyTypes;

typedef enum EnemyState
{
    EnemyState_Idle = 0,
    EnemyState_Follow = 1,
    EnemyState_Attack = 2,
    EnemyState_Death = 3,
    EnemyState_IdleRange = 4,
    EnemyState_FollowRange = 5,
    EnemyState_AttackRange = 6,
    EnemyState_DeathRange = 7,
    EnemyState_Size = 8
} EnemyState;

typedef struct Enemy Enemy;

typedef struct EnemyBehaviour
{
    void (*act)(Map* map, Player* player, Enemy *enemy);
    EnemyState state;
} EnemyBehaviour;

struct Enemy
{
    v2 pos, momentum, dir_to_player;
    float32 hitbox_radius, speed, len_to_player;
    uint state_frame;
    struct EnemyBehaviour *state;
    enum EnemyTypes type;
    bool is_alive;
    // Game variables
    float32 hp, max_hp;
};
PTR_TYPE(Enemy);
vec_register(Enemyptr);
vec_register(Enemy);
vec_register(EnemyBehaviour);

//Globals
extern int half_w, half_h;
extern uint frames;

static bool tile_is_wall(Map* map, int x, int y)
{
    const int index = y * map->w + x;
    return map->data[index] != 0 &&
           map->data[index] != 'P' &&
           map->data[index] != 'E';
}

static bool near_player(v2 entity_pos, v2 player_pos, int range)
{
    vi2 player_chunk_pos = {player_pos[0] / C_CHUNK_W, player_pos[1] / C_CHUNK_H};
    vi2 entity_chunk_pos = {entity_pos[0] / C_CHUNK_W, entity_pos[1] / C_CHUNK_H};
    return ((abs(player_chunk_pos[0] - entity_chunk_pos[0]) +
            abs(player_chunk_pos[1] - entity_chunk_pos[1])) <= range);
}

static void linecast(Map* map, int start_x, int start_y, int end_x, int end_y, int *wall_x, int *wall_y, bool (*f)(Map* map, int, int))
{
    int dx = abs(end_x - start_x), sx = start_x < end_x ? 1 : -1;
    int dy = -abs(end_y - start_y), sy = start_y < end_y ? 1 : -1;
    int err = dx + dy, e2; //error value e_xy
    while (1)
    { // loop
        if (start_x >= 0 && start_x < map->w && start_y >= 0 && start_y < map->h)
        {
            if (f(map, start_x, start_y))
            {
                *wall_x = start_x;
                *wall_y = start_y;
            }
        }
        e2 = 2 * err;
        if (e2 >= dy)
        { // e_xy+e_x > 0
            if (start_x == end_x)
                break;
            err += dy;
            start_x += sx;
        }
        if (e2 <= dx)
        { // e_xy+e_y < 0
            if (start_y == end_y)
                break;
            err += dx;
            start_y += sy;
        }
    }
}

#endif