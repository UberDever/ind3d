#ifndef IND3D_CONFIG_H
#define IND3D_CONFIG_H

#define ARR_COPY_LIST(var, arr) memcpy(&(var), &(arr), sizeof(arr));

//Generic vector registers
vec_register(GLfloat);
vec_register(GLuint);

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

#define C_PLAYER_POS \
    (v2) { 1.5, 7 }
#define C_PLAYER_DIR \
    (v2) { 0, 1 }
#define C_PLAYER_PLANE \
    (v2) { 1, 0 }
#define C_PLAYER_DIR_SCALE 1
#define C_PLAYER_PLANE_SCALE 1 / 3.f
#define C_PLAYER_SHOOT_SCALE 1 / 25.f
#define C_PLAYER_ROTATION_SPEED DEG2RAD(3)
#define C_PLAYER_ANGULAR_MOMENTUM 0
#define C_PLAYER_SPEED 0.1F
#define C_PLAYER_HITBOX_RADIUS .45
#define C_PLAYER_MAX_HP 100
#define C_PLAYER_HITSCAN_RANGE 0.3f

typedef struct Map
{
    int *data;
    int w, h;
    int tile_w, tile_h;
} Map;

#define C_MAP_TILE_W 10
#define C_MAP_TILE_H 10

typedef struct Projectile
{
    v2 pos, momentum;
    float32 radius, speed;
    bool is_alive;
} Projectile;

#define C_PROJECTILE_RADIUS 0.3f
#define C_PROJECTILE_SPEED 0.1f

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

#define C_ENEMYTYPE_COMMON         \
    (Enemy)                        \
    {                              \
        .type = EnemyTypes_Common, \
        .speed = 0.05,             \
        .hitbox_radius = 0.45,     \
        .max_hp = 50               \
    }

#define C_ENEMYTYPE_STRONG         \
    (Enemy)                        \
    {                              \
        .type = EnemyTypes_Strong, \
        .speed = 0.05,             \
        .hitbox_radius = 0.45,     \
        .max_hp = 100              \
    }

#define C_ENEMYTYPE_BOSS         \
    (Enemy)                      \
    {                            \
        .type = EnemyTypes_Boss, \
        .speed = 0.05,           \
        .hitbox_radius = 0.45,   \
        .max_hp = 200            \
    }

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
    void (*act)(Enemy *enemy);
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

// Struct specific vector registers
PTR_TYPE(Enemy);
vec_register(Enemyptr);

vec_register(Projectile);
vec_register(Enemy);
vec_register(EnemyBehaviour);

#endif