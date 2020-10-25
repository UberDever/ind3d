#ifndef IND3D_CONFIG_H
#define IND3D_CONFIG_H

#define ARR_COPY_LIST(var, arr) memcpy(&(var), &(arr), sizeof(arr));

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

#define C_MAP_TILE_W 10
#define C_MAP_TILE_H 10

#define C_PROJECTILE_RADIUS 0.3f
#define C_PROJECTILE_SPEED 0.1f

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


#define C_CHUNK_W 32
#define C_CHUNK_H 32
#define C_CHUNK_DIST 1

#endif