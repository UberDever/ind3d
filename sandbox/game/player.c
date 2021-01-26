//
// Created by uberdever on 06.09.2020.
//

#include "player.h"


static Weapon weapons[C_PLAYER_WEAPON_COUNT];

void player_init(Player *player)
{
    player->momentum = (v2_t){};
    player->hitbox_radius = C_PLAYER_HITBOX_RADIUS;
    player->max_hp = C_PLAYER_MAX_HP;
    player->hp = C_PLAYER_MAX_HP;
    player->hitscan_range = C_PLAYER_HITSCAN_RANGE;
    player->shots_fired = false;

    //player->pos = (v2_t){.x = C_CHUNK_W / 2, .y = C_CHUNK_H / 2};
    player->pos = (v2_t){.x = 2, .y = 2};
    player->camera.pos = (v3_t){player->pos.x, 0, player->pos.y};
    player->camera.dir = (v3_t){.x = 0, .y = 0, .z = 1};
    player->camera.plane = (v3_t){.x = 1, .y = 0, .z = 0};
    player->camera.up = (v3_t){.x = 0, .y = 1, .z = 0};
    player->camera.horisontal_rotation = 0;
    player->camera.sensitivity = 0.0125;

    // player->right_frustum_ray.x = -1;
    // player->left_frustum_ray.x = -player->right_frustum_ray.x;
    // player->right_frustum_ray.y = tanf(M_PI_2 - C_CAMERA_FOV / 2);
    // player->left_frustum_ray.y = player->right_frustum_ray.y;

    player->camera.projection_matrix = m4_perspective(C_CAMERA_FOV,
                                                      (float32)g_scr.w / (float32)g_scr.h,
                                                      C_CAMERA_NEAR,
                                                      C_CAMERA_FAR);

    // v_GLfloat_t vertices;
    // v_GLuint_t indices;
    // obj_parse("graphics/models/teapot.obj", &vertices, &indices);
    //Texture tex = texture_load("graphics/textures/wall.png");
    // weapon = model_build(vertices, indices);
    // weapon.texture = tex;
    player->bullet_count = 3;
    player->cells_count = 0;

    weapons[0].model = voxel_sprite_construct(
        "graphics/sprites/hi_power.png",
        "graphics/depthmaps/hi_power.zaxis",
        false,
        (v3_t){0.1f, 0.1f, 0.1f});
    model_add_texture(&weapons[0].model, texture_load("graphics/sprites/hi_power.png"));
    model_add_texture(&weapons[0].model, texture_load("graphics/sprites/hi_power.png"));
    weapons[0].position = (v3_t){-.35, -.2, -.5};
    weapons[0].recoil_pos = (v3_t){-.35, -.15, -.6};
    weapons[0].recoil_speed = C_PLAYER_WEAPON_RECOIL_SPEED;
    weapons[0].return_speed = C_PLAYER_WEAPON_RETURN_SPEED;
    weapons[0].recoil_rotation = M_PI_4;
    weapons[0].is_recoil = false;
    weapons[0].cur_recoil_pos = weapons[0].position;
    weapons[0].cur_recoil_rotation = 0;
    weapons[0].cur_frame_inc = 1;
    weapons[0].bullets = &player->bullet_count;
    weapons[0].type = WeaponType_Pistol;

    weapons[1].model = voxel_sprite_construct(
        "graphics/sprites/tesla_gun.png",
        "graphics/depthmaps/tesla_gun.zaxis",
        false,
        (v3_t){0.1f, 0.1f, 0.1f});
    model_add_texture(&weapons[1].model, texture_load("graphics/sprites/tesla_gun.png"));
    model_add_texture(&weapons[1].model, texture_load("graphics/sprites/tesla_gun.png"));
    model_add_texture(&weapons[1].model, texture_load("graphics/sprites/tesla_gun_emissive.png"));
    weapons[1].position = (v3_t){-.25, -.2, -.55};
    weapons[1].recoil_pos = (v3_t){-.28, -.18, -.57};
    weapons[1].recoil_speed = C_PLAYER_WEAPON_RECOIL_SPEED;
    weapons[1].return_speed = C_PLAYER_WEAPON_RETURN_SPEED;
    weapons[1].recoil_rotation = DEG2RAD(7);
    weapons[1].is_recoil = false;
    weapons[1].cur_recoil_pos = weapons[1].position;
    weapons[1].cur_recoil_rotation = 0;
    weapons[1].cur_frame_inc = 1;
    weapons[1].bullets = &player->cells_count;
    weapons[1].type = WeaponType_TeslaGun;

    player->cur_weapon = &weapons[0];

}

void player_event(Player *player)
{
    int m_x = mouse_get_rel_x();
    int m_y = mouse_get_rel_y();

    player->camera.horisontal_rotation += (float32)m_x * player->camera.sensitivity;
    player->camera.vertrical_rotation += -(float32)m_y * player->camera.sensitivity;

    const float32 offsetted_pi = M_PI_2 - 0.1745f;
    if (player->camera.vertrical_rotation > offsetted_pi)
        player->camera.vertrical_rotation = offsetted_pi;
    else if (player->camera.vertrical_rotation < -offsetted_pi)
        player->camera.vertrical_rotation = -offsetted_pi;
    if (player->camera.horisontal_rotation < 0)
        player->camera.horisontal_rotation = 2 * M_PI;
    else if (player->camera.horisontal_rotation > 2 * M_PI)
        player->camera.horisontal_rotation = 0;

    m4_t rotation_dir = m4_mul(m4_rotation_y(-player->camera.horisontal_rotation),
                               m4_rotation_x(-player->camera.vertrical_rotation));
    player->camera.dir = m4_mul_dir(rotation_dir, (v3_t){0, 0, 1});
    player->camera.plane = m4_mul_dir(rotation_dir, (v3_t){1, 0, 0});
    player->camera.up = m4_mul_dir(rotation_dir, (v3_t){0, 1, 0});

    player->dir = v2_norm((v2_t){player->camera.dir.x, player->camera.dir.z});
    player->plane = v2_norm((v2_t){player->camera.plane.x, player->camera.plane.z});

    m4_t rotation_frustum = m4_rotation_y(C_CAMERA_FOV / 2);
    v3_t left_frustum = m4_mul_dir(rotation_frustum, player->camera.dir);
    left_frustum = v3_norm((v3_t){left_frustum.x, 0, left_frustum.z});
    player->left_frustum = (v2_t){left_frustum.x, left_frustum.z};
    rotation_frustum = m4_rotation_y(-C_CAMERA_FOV);
    v3_t right_frustum = m4_mul_dir(rotation_frustum, left_frustum);
    player->right_frustum = (v2_t){right_frustum.x, right_frustum.z};

    if (kbd_key_pressed(SDLK_w))
    {
        player->momentum.y = C_PLAYER_SPEED;
    }
    if (kbd_key_pressed(SDLK_a))
    {
        player->momentum.x = C_PLAYER_SPEED;
    }
    if (kbd_key_pressed(SDLK_s))
    {
        player->momentum.y = -C_PLAYER_SPEED;
    }
    if (kbd_key_pressed(SDLK_d))
    {
        player->momentum.x = -C_PLAYER_SPEED;
    }

    if (kbd_key_pressed(SDLK_1))
    {
        player->cur_weapon = &weapons[0];
    }
    if (kbd_key_pressed(SDLK_2))
    {
        player->cur_weapon = &weapons[1];
    }

    if (is_player_exited)
    {
        if (kbd_key_pushed(SDLK_ESCAPE))
        {
            is_program_running = false;
        }
    }
    //if (is_player_dead)
    //{
        if (kbd_key_pushed(SDLK_ESCAPE))
        {
            is_program_running = false;
        }
    //}
    if (is_beginning)
    {
        if (kbd_key_pushed(SDLK_SPACE))
        {
            is_beginning = false;
        }
    }
}

void player_update(Map *map, Player *player)
{
#if 0
    const m2 dir_rotation = {0, -1, 1, 0};
    pi_v2(player->dir, mouse_get_pos_x() - (half_w), mouse_get_pos_y() - half_h);
    pi_v2_mul_m2(dir_rotation, player->dir, player->plane);
    pi_v2_normalize(player->dir, player->dir);
    pi_v2_normalize(player->plane, player->plane);
#endif

    //v2 left_frustum, right_frustum;
    //pi_v2_muls(player->camera.dir, C_PLAYER_DIR_SCALE, right_frustum);
    //pi_v2_muls(player->camera.plane, C_PLAYER_PLANE_SCALE, left_frustum);
    //pi_v2_add(right_frustum, left_frustum, player->left_frustum_ray);
    //left_frustum.x *= -1;
    //left_frustum.y *= -1;
    //pi_v2_add(right_frustum, left_frustum, player->right_frustum_ray);
    //
    //v2 tmp0 = {};
    //pi_v2_muls(player->dir, player->momentum.y, tmp0);
    //v2 tmp1 = {};
    //pi_v2_muls(player->plane, player->momentum.x, tmp1);
    //v2 step = {};a
    //pi_v2_add(tmp0, tmp1, step);

    v2_t dir = (v2_t){player->camera.dir.x, player->camera.dir.z};
    v2_t plane = (v2_t){player->camera.plane.x, player->camera.plane.z};
    v2_t step = v2_add(v2_muls(v2_norm(dir), player->momentum.y), v2_muls(v2_norm(plane), player->momentum.x));
    v2_t test_move = {};
    test_move.x = step.x >= 0 ? player->hitbox_radius : -player->hitbox_radius;
    test_move.y = step.y >= 0 ? player->hitbox_radius : -player->hitbox_radius;
    v2_t test_position = v2_add(player->pos, step);
    if (tile_is_wall(map, test_position.x + test_move.x, test_position.y))
        step.x = 0;
    if (tile_is_wall(map, test_position.x, test_position.y + test_move.y))
        step.y = 0;
    test_move = v2_muls(test_move, SQRT2_2);
    if (tile_is_wall(map, test_position.x + test_move.x, test_position.y - test_move.y))
        step.x = 0;
    if (tile_is_wall(map, test_position.x - test_move.x, test_position.y + test_move.y))
        step.y = 0;
    if (tile_is_wall(map, test_position.x + test_move.x, test_position.y + test_move.y))
    {
        step.x = 0;
        step.y = 0;
    }
    player->pos = v2_add(player->pos, step);
    player->camera.pos = (v3_t){player->pos.x, 0, player->pos.y};

    player->momentum.x = 0;
    player->momentum.y = 0;

    if (map->data[(int)player->pos.y * map->w + (int)player->pos.x] == 'M')
    {
        player->hp += C_PLAYER_MEDPACK_HEALTH;
        if (player->hp > player->max_hp)
            player->hp = player->max_hp;
        for (int i = 0; i < map->medpack_positions.size; i++)
        {
            if ((int)player->pos.x + .5 == map->medpack_positions.data[i].x &&
                (int)player->pos.y + .5 == map->medpack_positions.data[i].z)
            {
                map->medpack_positions.data[i].y = 1;
            }
        }
    }
    else if (map->data[(int)player->pos.y * map->w + (int)player->pos.x] == 'B')
    {
        player->bullet_count += C_PLAYER_BULLET_COUNT;
        for (int i = 0; i < map->bullets_positions.size; i++)
        {
            if ((int)player->pos.x + .5 == map->bullets_positions.data[i].x &&
                (int)player->pos.y + .5 == map->bullets_positions.data[i].z)
            {
                map->bullets_positions.data[i].y = 1;
            }
        }
    }
    else if (map->data[(int)player->pos.y * map->w + (int)player->pos.x] == 'C')
    {
        player->cells_count += C_PLAYER_CELLS_COUNT;
        for (int i = 0; i < map->cells_positions.size; i++)
        {
            if ((int)player->pos.x + .5 == map->cells_positions.data[i].x &&
                (int)player->pos.y + .5 == map->cells_positions.data[i].z)
            {
                map->cells_positions.data[i].y = 1;
            }
        }
    }
    else if (map->data[(int)player->pos.y * map->w + (int)player->pos.x] == 'X')
    {
        player->has_red_key = true;
        map->keys_position[0].y = 1;
    }
    else if (map->data[(int)player->pos.y * map->w + (int)player->pos.x] == 'Y')
    {
        player->has_green_key = true;
        map->keys_position[1].y = 1;
    }
    else if (map->data[(int)player->pos.y * map->w + (int)player->pos.x] == 'Z')
    {
        player->has_blue_key = true;
        map->keys_position[2].y = 1;
    }
    else if (map->data[(int)player->pos.y * map->w + (int)player->pos.x] == 'E') //Exit door
    {
        is_player_exited = true;
    }

    map->data[(int)player->pos.y * map->w + (int)player->pos.x] = 'P';

    if (player->cur_weapon->is_recoil)
    {
        player->cur_weapon->cur_recoil_pos = v3_lerp(player->cur_weapon->position,
                                                     player->cur_weapon->recoil_pos,
                                                     (float)player->cur_weapon->cur_frame / C_PLAYER_WEAPON_RECOIL_SPEED);
        player->cur_weapon->cur_recoil_rotation =
            pi_lerp(0, player->cur_weapon->recoil_rotation, (float)player->cur_weapon->cur_frame / C_PLAYER_WEAPON_RECOIL_ROTATION_SPEED);
        player->cur_weapon->cur_frame += player->cur_weapon->cur_frame_inc;
        if (player->cur_weapon->cur_recoil_pos.x == player->cur_weapon->recoil_pos.x &&
            player->cur_weapon->cur_recoil_pos.y == player->cur_weapon->recoil_pos.y &&
            player->cur_weapon->cur_recoil_pos.z == player->cur_weapon->recoil_pos.z)
        {
            player->cur_weapon->cur_frame_inc = -1;
        }
        else if (player->cur_weapon->cur_recoil_pos.x == player->cur_weapon->position.x &&
                 player->cur_weapon->cur_recoil_pos.y == player->cur_weapon->position.y &&
                 player->cur_weapon->cur_recoil_pos.z == player->cur_weapon->position.z &&
                 player->cur_weapon->cur_frame_inc == -1)
        {
            player->cur_weapon->cur_frame_inc = 1;
            player->cur_weapon->cur_recoil_pos = player->cur_weapon->position;
            player->cur_weapon->cur_recoil_rotation = 0;
            player->cur_weapon->is_recoil = false;
            player->cur_weapon->cur_frame = 0;
        }
    }
}

void player_raycast(Map *map, Player *player)
{ //TODO: REBUILD
#if 0
    int scrW = g_scr.w;
    int scrH = g_scr.h;

    int dS = 0;
    int dE = 0;

    g_screen_fill(COLOR(black));

    for (int x = 0; x < scrW; ++x)
    {
        float32 cam = 2.f * (float)x / (float)scrW - 1;
        v2 ray;
        pi_v2(ray, -player->plane.x, -player->plane.y);
        pi_v2_muls(ray, cam, ray);
        pi_v2_add(ray, player->dir, ray);
        vi2 pos = {player->pos.x, player->pos.y};
        v2 side;
        v2 delta = {fabsf(1.f / ray.x), fabsf(1.f / ray.y)};
        float32 pwd = 0;
        vi2 step;
        bool hit = false, NS = true; //North-South

        if (ray.x > 0)
        {
            step.x = 1;
            side.x = ((float32)pos.x + 1 - player->pos.x) * delta.x;
        }
        else
        {
            step.x = -1;
            side.x = (player->pos.x - pos.x) * delta.x;
        }
        if (ray.y > 0)
        {
            step.y = 1;
            side.y = ((float32)pos.y + 1 - player->pos.y) * delta.y;
        }
        else
        {
            step.y = -1;
            side.y = (player->pos.y - pos.y) * delta.y;
        }
        while (!hit)
        {
            if (side.x < side.y)
            {
                side.x += delta.x;
                pos.x += step.x;
                NS = true;
            }
            else
            {
                side.y += delta.y;
                pos.y += step.y;
                NS = false;
            }
            if (map->data[pos.y * map->w + pos.x])
                hit = true;
        }

        if (NS)
            pwd = (pos.x - player->pos.x + (1.f - step.x) / 2) / ray.x;
        else
            pwd = (pos.y - player->pos.y + (1.f - step.y) / 2) / ray.y;

        int line = (int)(2 * (float32)scrH / pwd);
        if (line) //for evading zero-length lines
        {
            dS = -line / 2 + (scrH) / 2;
            dS = dS < 0 ? 0 : dS;
            dE = line / 2 + (scrH) / 2;
            dE = dE >= scrH ? scrH - 1 : dE;
            //dS = -line / 2 + scrH / 2 - pd->pl->offset; dS = dS < 0 ? 0 : dS;
            //dE = line / 2 + scrH / 2 - pd->pl->offset; dE = dE >= scrH ? scrH - 1 : dE;
        }

        g_screen_draw_line(x, dS, x, dE, COLOR(cyan));
    }
    //SDL_Rect o = {0, 0, g_scr.w, g_scr.h};
    //SDL_UpdateTexture(global::gameScreen(), 0, global::gameScreenBuffer(), sizeof(uint32_t) * global::cfg::scrRenW());
    ////SDL_RenderCopy(global::renderer(), skybox, 0, &o);
    //SDL_RenderCopy(global::renderer(), global::gameScreen(), 0, 0);
    //SDL_RenderPresent(global::renderer());
#endif
}

void player_clean(Player *player)
{
}

void player_render(Player *player)
{
#if 1 // Draw weapons
    const m4_t basis = mat4(player->camera.plane.x, player->camera.up.x, player->camera.dir.x, 0,
                            player->camera.plane.y, player->camera.up.y, player->camera.dir.y, 0,
                            player->camera.plane.z, player->camera.up.z, player->camera.dir.z, 0,
                            0, 0, 0, 1);

    //const v3_t tr_world = {};
    const v3_t tr_world = {player->pos.x + player->camera.dir.x,
                           player->camera.dir.y,
                           player->pos.y + player->camera.dir.z};
    const quaternion_t ro_world = quaternion_axis((v3_t){0, 1, 0}, -M_PI_2);

    const v3_t sc_world = {.1, .1, .1};

    v3_t tr_basis = player->cur_weapon->cur_recoil_pos;

    tr_basis = m4_mul_pos(basis, tr_basis);

    const v3_t translation = v3_add(tr_world, tr_basis);
    const quaternion_t rotation = quaternion_mul(quaternion_axis((v3_t){-1, 0, 0}, player->cur_weapon->cur_recoil_rotation), ro_world);
    const v3_t scaling = sc_world;

    const m4_t tr_m4 = m4_mul(m4_translation(translation), basis);
    const m4_t ro_m4 = quaternion_rot_mat(rotation);
    const m4_t sc_m4 = m4_scaling(scaling);

    ModelInstance instance = MI(player->cur_weapon->model, m4_mul(tr_m4, m4_mul(ro_m4, sc_m4)));

    renderer_add(player->cur_weapon->model.info.vao, instance);

    glClear(GL_DEPTH_BUFFER_BIT);
    renderer_render(player->cur_weapon->model); // Immidiate render, because of clear depth buffer bit

    renderer_clear_model(player->cur_weapon->model.info.vao);
#endif

#if 0
    v3_t tr_world = {player->pos.x + (player->camera.dir.x), player_height_debug + player->camera.dir.y, player->pos.y + player->camera.dir.z};
    quaternion_t ro_world = quaternion_axis((v3_t){0, 1, 0}, -M_PI_2);
    v3_t sc_world = {.1, .1, .1};

    v3_t tr_basis = {.2, 0, 0};
    tr_basis = m4_mul_pos(basis, tr_basis);
    quaternion_t ro_basis = quaternion_axis(player->camera.dir, DEG2RAD(90));

    const v3_t translation = v3_add(tr_world, tr_basis);      // (v3_t){1, 1, 1};
    const quaternion_t rotation = quaternion_mul(ro_world, ro_basis); // (quaternion_t){0, 0, 0, 1}; // ro_world;
    const v3_t scaling = sc_world;         ////sc_world;

    const m4_t tr_m4 = m4_mul(m4_translation(translation), basis);
    const m4_t ro_m4 = m4_mul(quaternion_rot_mat(rotation), basis);
    const m4_t sc_m4 = m4_scaling(scaling);

    weapon0_instance.transform = m4_mul(tr_m4, m4_mul(ro_m4, sc_m4));
#endif
}
