#ifndef IND3D_CONFIG_H
#define IND3D_CONFIG_H

#define ARR_COPY_LIST(var, arr) memcpy(&(var), &(arr), sizeof(arr));

#define C_PLAYER_POS_X 2
#define C_PLAYER_POS_Y 2
#define C_PLAYER_DIR \
    (v2_t) { .x = 0, .y = 1 }
#define C_PLAYER_PLANE \
    (v2_t) { .x = 1, .y = 0 }
#define C_PLAYER_DIR_SCALE 1
#define C_PLAYER_PLANE_SCALE 1 / 3.f
#define C_PLAYER_SHOOT_SCALE 1 / 25.f
#define C_PLAYER_ROTATION_SPEED DEG2RAD(3)
#define C_PLAYER_ANGULAR_MOMENTUM 0
#define C_PLAYER_SPEED 0.1F
#define C_PLAYER_HITBOX_RADIUS .45
#define C_PLAYER_MAX_HP 100
#define C_PLAYER_HITSCAN_RANGE 0.3f
#define C_PLAYER_WEAPON_COUNT 4
#define C_PLAYER_MEDPACK_HEALTH 25
#define C_PLAYER_BULLET_COUNT 2
#define C_PLAYER_CELLS_COUNT 1
#define C_PLAYER_WEAPON_RECOIL_ROTATION_SPEED 15 //in frames
#define C_PLAYER_WEAPON_RECOIL_SPEED 15          //in frames
#define C_PLAYER_WEAPON_RETURN_SPEED 60

#define C_CAMERA_FOV 90
#define C_CAMERA_NEAR 0.1F
#define C_CAMERA_FAR 1000

#define C_MAP_TILE_RATIO_W (1.f / 100)
#define C_MAP_TILE_RATIO_H (1.f / 100)

#define C_PROJECTILE_RADIUS 0.1f
#define C_PROJECTILE_SPEED 0.1f

#define C_ENEMYTYPE_COMMON                    \
    (Enemy)                                   \
    {                                         \
        .type = EnemyTypes_Common,            \
        .speed = 0.05,                        \
        .hitbox_radius = 0.2,                \
        .max_hp = 50,                         \
        .damage = 10,                         \
        .model = voxel_sprite_construct(      \
            "graphics/sprites/robot_all.png", \
            "graphics/depthmaps/robot.zaxis", \
            true,                             \
            (v3_t){0.1f, 0.1f, 0.1f})         \
    }

#define C_ENEMYTYPE_STRONG                    \
    (Enemy)                                   \
    {                                         \
        .type = EnemyTypes_Strong,            \
        .speed = 0.05,                        \
        .hitbox_radius = 0.45,                \
        .max_hp = 100,                        \
        .model = voxel_sprite_construct(      \
            "graphics/sprites/robot_all.png", \
            "graphics/depthmaps/robot.zaxis", \
            true,                             \
            (v3_t){0.1f, 0.1f, 0.1f})         \
    }

#define C_ENEMYTYPE_BOSS                      \
    (Enemy)                                   \
    {                                         \
        .type = EnemyTypes_Boss,              \
        .speed = 0.05,                        \
        .hitbox_radius = 0.45,                \
        .max_hp = 200,                        \
        .model = voxel_sprite_construct(      \
            "graphics/sprites/robot_all.png", \
            "graphics/depthmaps/robot.zaxis", \
            true,                             \
            (v3_t){0.1f, 0.1f, 0.1f})         \
    }

#define C_DOOR_SPEED 0.03f

#define C_CHUNK_W 32
#define C_CHUNK_H 32
#define C_CHUNK_DIST 1
static int C_RANGE = 6;

#define C_D_AMBIENT ((v3_t){0.15f, 0.15f, 0.15f})
#define C_D_DIFFUSE ((v3_t){0.45f, 0.45f, 0.45f})
#define C_D_SPECULAR ((v3_t){0.85f, 0.85f, 0.85f})

#define C_P_AMBIENT ((v3_t){0.1, 0.1, 0.1})
#define C_P_DIFFUSE ((v3_t){0.7, 0.7, 0.7})
#define C_P_SPECULAR ((v3_t){1, 1, 1})
#define C_P_ATTENUATION ((v3_t){0.128, 0.09, 0.8})

#define C_RED ((v3_t){1, 0, 0})
#define C_ORANGE ((v3_t){0.99, 0.53, 0})
#define C_YELLOW ((v3_t){1, 1, 0})
#define C_GREEN ((v3_t){0, 1, 0})
#define C_CYAN ((v3_t){.345, 0.8, 0.92})
#define C_BLUE ((v3_t){0, 0, 1})
#define C_PURPLE ((v3_t){.38, 0.185, 0.42})

#endif