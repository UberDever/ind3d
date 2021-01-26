//
// Created by uberdever on 06.09.2020.
//
#ifndef IND3D_CORE_H
#define IND3D_CORE_H

#include "../include/alphabeta.h"

#include "gl/geometry.h"
#include "gl/shader.h"
#include "gl/model.h"
#include "gl/renderer.h"
#include "gl/line_renderer.h"
#include "gl/plane_renderer.h"

#include "gl/voxel_sprite.h"

#include "config.h"

//Global typedefs
typedef enum
{
    WeaponType_Pistol,
    WeaponType_TeslaGun
} WeaponTypes;

typedef struct Weapon
{
    WeaponTypes type;
    v3_t position;
    v3_t recoil_pos;
    float recoil_speed;
    float return_speed;
    float recoil_rotation;
    v3_t cur_recoil_pos;
    float cur_recoil_rotation;
    bool is_recoil;
    int cur_frame;
    int cur_frame_inc;
    int *bullets;
    Model model;
} Weapon;
vec_register(Weapon);

typedef struct Player
{
    v2_t pos, momentum;
    v2_t dir, plane; // This is very bad design - almost never mix 2d and 3d together, good lesson
    v2_t left_frustum, right_frustum;
    float32 hitbox_radius;
    Camera camera;
    // Game variables
    float32 hp, max_hp;
    float32 hitscan_range;
    int bullet_count;
    int cells_count;
    bool shots_fired;
    bool has_red_key, has_green_key, has_blue_key;

    Weapon *cur_weapon;
} Player;

typedef struct Map
{
    int *data;
    int w, h;
    int tile_w, tile_h;
    // OpenGl stuff
    int chunks_w, chunks_h;
    v_Model_t chunks;
    Texture tileset;
    vec_v3_t light_positions;
    vec_v3_t door_positions;
    vec_v3_t medpack_positions;
    vec_v3_t bullets_positions;
    vec_v3_t cells_positions;
    v3_t exit_position;
    v3_t keys_position[3];
} Map;

typedef struct Projectile
{
    v2_t pos, momentum;
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
    void (*act)(Map *map, Player *player, Enemy *enemy);
    EnemyState state;
} EnemyBehaviour;

struct Enemy
{
    Model model;
    v2_t pos, momentum, dir_to_player;
    float32 hitbox_radius, speed, len_to_player;
    uint state_frame;
    struct EnemyBehaviour *state;
    enum EnemyTypes type;
    bool is_alive;
    // Game variables
    float32 hp, max_hp;
    float32 damage;
};
PTR_TYPE(Enemy);
vec_register(Enemyptr);
vec_register(Enemy);
vec_register(EnemyBehaviour);

//Globals
extern int half_w, half_h;
extern uint frames;
extern suseconds_t framerate;
extern bool is_player_exited;
extern bool is_all_enemies_dead;
extern bool is_player_dead;
extern bool is_beginning;

extern Shader shader;
extern Shader light_shader;

extern PointLight point_light;

// TODO: remove later
extern float player_height_debug;

static bool tile_is_wall(Map *map, int x, int y)
{
    const int index = y * map->w + x;
    return map->data[index] != 0 &&
           map->data[index] != 'R' - '0' &&
           map->data[index] != 'D' - '0' &&
           map->data[index] != '3' - '0' &&
           map->data[index] != 'M' - '0' &&
           map->data[index] != 'B' - '0' &&
           map->data[index] != 'C' - '0' &&
           map->data[index] != 'X' - '0' &&
           map->data[index] != 'Y' - '0' &&
           map->data[index] != 'Z' - '0' &&
           map->data[index] != 'P' &&
           map->data[index] != 'M' &&
           map->data[index] != 'B' &&
           map->data[index] != 'C' &&
           map->data[index] != 'E' &&
           map->data[index] != 'X' &&
           map->data[index] != 'Y' &&
           map->data[index] != 'Z' &&
           map->data[index] != 'L' - '0';
}

static bool tile_is_collectable(Map *map, int x, int y)
{
    const int index = y * map->w + x;
    return map->data[index] == 'M' ||
           map->data[index] == 'C' ||
           map->data[index] == 'X' ||
           map->data[index] == 'Y' ||
           map->data[index] == 'Z' ||
           map->data[index] == 'B';
}

static bool near_player(v2_t entity_pos, v2_t player_pos, int range)
{
    int player_chunk_pos_x = player_pos.x / C_CHUNK_W, player_chunk_pos_y = player_pos.y / C_CHUNK_H;
    int entity_chunk_pos_x = entity_pos.x / C_CHUNK_W, entity_chunk_pos_y = entity_pos.y / C_CHUNK_H;
    return ((abs(player_chunk_pos_x - entity_chunk_pos_x) +
             abs(player_chunk_pos_y - entity_chunk_pos_y)) <= range);
}

static void linecast(Map *map, int start_x, int start_y, int end_x, int end_y, int *wall_x, int *wall_y, bool (*f)(Map *map, int, int))
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