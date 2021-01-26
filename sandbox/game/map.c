//
// Created by uberdever on 06.09.2020.
//

#include "map.h"

Texture map_floor_texture = {0};
Model map_floor = {};
Model map_ceiling = {};

static void map_chunks_init(Map *map, const char *tileset_path)
{
    vec_new(v3_t, map->light_positions, 16);
    vec_new(v3_t, map->door_positions, 16);
    vec_new(v3_t, map->medpack_positions, 16);
    vec_new(v3_t, map->bullets_positions, 16);
    vec_new(v3_t, map->cells_positions, 16);

    map->chunks_w = map->w / C_CHUNK_W;
    map->chunks_h = map->h / C_CHUNK_H;
    vec_new(Model, map->chunks, map->chunks_w * map->chunks_h);

    v_GLfloat_t vertices = {};
    v_GLuint_t indices = {};
    vec_new(GLfloat, vertices, C_CHUNK_W * C_CHUNK_H / 4 * 8 * model_vx_count);
    vec_new(GLuint, indices, C_CHUNK_W * C_CHUNK_H / 4 * 8 * 3);
    int ch_half_w = C_CHUNK_W / 2;
    int ch_half_h = C_CHUNK_H / 2;
    GLuint cur_pube_idx = 0;

    map->tileset = texture_load(tileset_path);

    for (int i = 0; i < map->chunks_h; i++)
    {
        for (int j = 0; j < map->chunks_w; j++)
        {
            Model chunk = {};

            for (int y = -0; y < C_CHUNK_H; y++)
            {
                int map_y = j * C_CHUNK_H + y;
                for (int x = -0; x < C_CHUNK_W; x++)
                {
                    int map_x = i * C_CHUNK_W + x;
                    bool generate = false;
                    if (map->data[map_y * map->w + map_x] == 'L' - '0')
                    {
                        vec_push(map->light_positions, ((v3_t){map_x + .5, 0.5f, map_y + .5}));
                        map->data[map_y * map->w + map_x] = 0;
                        //continue;
                    }
                    else if (map->data[map_y * map->w + map_x] == 'D' - '0')
                    {
                        vec_push(map->door_positions, ((v3_t){map_x + .5, 0, map_y + .5}));
                        map->data[map_y * map->w + map_x] = 0;
                        //continue;
                    }
                    else if (map->data[map_y * map->w + map_x] == '3' - '0')
                    {
                        map->exit_position = (v3_t){map_x + .5, 0, map_y + .5};
                        generate = true;
                    }
                    else if (map->data[map_y * map->w + map_x] == 'M' - '0')
                    {
                        vec_push(map->medpack_positions, ((v3_t){map_x + .5, -0.4, map_y + .5}));
                        map->data[map_y * map->w + map_x] = 'M';
                    }
                    else if (map->data[map_y * map->w + map_x] == 'B' - '0')
                    {
                        vec_push(map->bullets_positions, ((v3_t){map_x + .5, -0.4, map_y + .5}));
                        map->data[map_y * map->w + map_x] = 'B';
                    }
                    else if (map->data[map_y * map->w + map_x] == 'C' - '0')
                    {
                        vec_push(map->cells_positions, ((v3_t){map_x + .5, -0.4, map_y + .5}));
                        map->data[map_y * map->w + map_x] = 'C';
                    }
                    else if (map->data[map_y * map->w + map_x] == 'X' - '0')
                    {
                        map->keys_position[0] =  ((v3_t){map_x + .5, -0.4, map_y + .5});
                        map->data[map_y * map->w + map_x] = 'X';
                    }
                    else if (map->data[map_y * map->w + map_x] == 'Y' - '0')
                    {
                        map->keys_position[1] = ((v3_t){map_x + .5, -0.4, map_y + .5});
                        map->data[map_y * map->w + map_x] = 'Y';
                    }
                    else if (map->data[map_y * map->w + map_x] == 'Z' - '0')
                    {
                        map->keys_position[2] = ((v3_t){map_x + .5, -0.4, map_y + .5});
                        map->data[map_y * map->w + map_x] = 'Z';
                    }
                    if (tile_is_wall(map, map_x, map_y) || generate)
                    {
                        bool adj_check[4] = {1, 1, 1, 1};
                        if (map_y - 1 < 0)
                            adj_check[0] = false;
                        else if (tile_is_wall(map, map_x, map_y - 1))
                            adj_check[0] = false;
                        if (map_x + 1 >= map->w)
                            adj_check[1] = false;
                        else if (tile_is_wall(map, map_x + 1, map_y))
                            adj_check[1] = false;
                        if (map_y + 1 >= map->h)
                            adj_check[2] = false;
                        else if (tile_is_wall(map, map_x, map_y + 1))
                            adj_check[2] = false;
                        if (map_x - 1 < 0)
                            adj_check[3] = false;
                        else if (tile_is_wall(map, map_x - 1, map_y))
                            adj_check[3] = false;

                        int map_tile = map->data[map_y * map->w + map_x] - 1 < 0 ? 0 : map->data[map_y * map->w + map_x] - 1;
                        generate_pube(&vertices, &indices, adj_check, x, y,
                                      (v2_t){map_tile / 16, map_tile % 16}, cur_pube_idx);

                        cur_pube_idx++;
                    }
                }
            }
            for (int idx = 0; idx < vertices.size; idx += model_vx_count)
            {
                vertices.data[idx + 0] += i * C_CHUNK_W + .5;
                vertices.data[idx + 1] += 0;
                vertices.data[idx + 2] += j * C_CHUNK_H + .5;
            }
            Mesh mesh = {};
            mesh.vertices = vertices;
            mesh.indices = indices;
            chunk = model_build(mesh);

#if 0
            {
                for (uint k = 0; k < vertices.size; k += 8)
                {
                    debug("%d. p: %f %f %f t: %f %f n: %f %f %f", k / 8, vertices.data[k], vertices.data[k + 1], vertices.data[k + 2], vertices.data[k + 3],
                          vertices.data[k + 4], vertices.data[k + 5], vertices.data[k + 6], vertices.data[k + 7]);
                }

                for (uint k = 0; k < indices.size; k += 3)
                {
                    printf("%d. %d ", k / 3, indices.data[k]);
                    printf("%d ", indices.data[k + 1]);
                    printf("%d\n", indices.data[k + 2]);
                }
                debug("\n%d %d", vertices.size, indices.size);
            }
#endif
            vertices.size = 0;
            indices.size = 0;
            cur_pube_idx = 0;
            vec_push(map->chunks, chunk);
        }
    }
}

void map_init(Map *map, const char *path, const char *tileset_path)
{
    map->tile_w = C_MAP_TILE_RATIO_W * g_scr.h;
    map->tile_h = C_MAP_TILE_RATIO_H * g_scr.h;

    FILE *map_source = NULL;
    map_source = fopen(path, "r");
    if (!map_source)
        error("Cannot load map, path: %s", path);

    fseek(map_source, 0, SEEK_END);
    uint map_len = ftell(map_source) + 1;
    fseek(map_source, 0, SEEK_SET);
    for (; 1; map->w++)
    {
        if (getc(map_source) == '\n' || map->w >= 1000)
            break;
    }
    fseek(map_source, 0, SEEK_SET);
    map->h = map_len / (map->w + 1);
    //debug("%d %d %d", map->w, map->h, map_len)
    ARR_ALLOC(map->data, map_len);
    for (uint i = 0; i < map->h; i++)
    {
        for (uint j = 0; j < map->w; j++)
        {
            map->data[i * map->w + j] = getc(map_source) - '0';
            //printf("%d", map->data[i * map->w + j]);
        }
        getc(map_source);
        //printf("\n");
    }
    map_chunks_init(map, tileset_path);
    map_floor_texture = texture_load("graphics/textures/minimap_floor.png");

    int floor_size_x = map->w;
    int floor_size_y = map->h;
    Mesh floorMesh = generate_plane((v3_t){0, -0.5, 0},
                                    (v3_t){floor_size_x, -0.5, 0},
                                    (v3_t){floor_size_x, -0.5, floor_size_y},
                                    (v3_t){0, -0.5, floor_size_y});
    map_floor = model_build(floorMesh);
    model_add_texture(&map_floor, texture_load("graphics/textures/floor_tiled.png"));
    model_add_texture(&map_floor, texture_load("graphics/textures/floor_tiled.png"));

    Mesh ceilingMesh = generate_plane((v3_t){0, 0.5, 0},
                                      (v3_t){floor_size_x, 0.5, 0},
                                      (v3_t){floor_size_x, 0.5, floor_size_y},
                                      (v3_t){0, 0.5, floor_size_y});
    map_ceiling = model_build(ceilingMesh);
    model_add_texture(&map_ceiling, texture_load("graphics/textures/floor_tiled.png"));
    model_add_texture(&map_ceiling, texture_load("graphics/textures/floor_tiled.png"));
}

void map_render(Map *map, Player *player, v_Enemy_t *enemies, v_Projectile_t *projectiles, const int pos_x, const int pos_y, const int size_x, const int size_y)
{
#if 0
    //TODO REMOVE
    glUseProgram(model_shader);
    uniform_m4(model_projection, player->camera.projection_matrix);
    v3_t view_pos = (v3_t){};
    view_pos.x = fmodf(player->pos.x, C_CHUNK_W) - C_CHUNK_W / 2 - .5f;
    view_pos.y = player_height_debug; //flight mode
    view_pos.z = fmodf(player->pos.y, C_CHUNK_H) - C_CHUNK_H / 2 - .5f;
    m4_t weapon_model = m4_identity(); //m4_mul(m4_translation((v3_t){view_pos.x - 1, -.3, view_pos.z + 1.3}), m4_rotation_y(DEG2RAD(100)));
    player->camera.view_matrix = m4_mul(m4_look_at(view_pos,
                                                   player->camera.dir,
                                                   (v3_t){0, 1, 0}),
                                        weapon_model);
    uniform_m4(model_worldView, player->camera.view_matrix);
    glBindTexture(GL_TEXTURE_2D, weapon_tex);
    glBindVertexArray(weapon.info.vao);
    glDrawElements(GL_TRIANGLES, weapon.info.idx_count, GL_UNSIGNED_INT, 0); //_ADJACENCY

    m4_t robot_model = m4_translation((v3_t){7, 0, 0});
    glBindTexture(GL_TEXTURE_2D, robot_tex);
    player->camera.view_matrix = m4_mul(m4_look_at(view_pos,
                                                   player->camera.dir,
                                                   (v3_t){0, 1, 0}),
                                        robot_model);
    uniform_m4(model_worldView, player->camera.view_matrix);
    glBindVertexArray(robot.info.vao);
    glDrawElements(GL_TRIANGLES, robot.info.idx_count, GL_UNSIGNED_INT, 0); //_ADJACENCY
#endif
    /*
        Chunks
        Drawn in low-level fashion, because share all texture info
    */
    int player_pos_in_chunks_x = (int)player->pos.x / C_CHUNK_W;
    int player_pos_in_chunks_y = (int)player->pos.y / C_CHUNK_H;

    shader_set_m4(&shader, u_model_name, m4_identity());  // Model is not needed for now
    shader_set_m4(&shader, u_iModel_name, m4_identity()); // iModel is not needed for now

    shader_bind_texture(shader, map->tileset.id, 0);
    shader_bind_texture(shader, map->tileset.id, 1);
    int index = 0;
    for (int x_rel = -C_RANGE; x_rel < C_RANGE; x_rel++)
    {
        for (int y_rel = -C_RANGE; y_rel < C_RANGE; y_rel++)
        {
            if (abs(x_rel) + abs(y_rel) <= C_RANGE)
            {
                int chunk_pos_x = player_pos_in_chunks_x + x_rel;
                int chunk_pos_y = player_pos_in_chunks_y + y_rel;
                if (pi_aabb_box_x_point(chunk_pos_x, chunk_pos_y, 0, 0, map->chunks_w, map->chunks_h))
                {
                    // v3_t view_pos;
                    // view_pos.x = fmodf(player->pos.x, C_CHUNK_W) - C_CHUNK_W / 2 - .5f - C_CHUNK_W * x_rel;
                    // view_pos.y = player_height_debug; //flight mode
                    // view_pos.z = fmodf(player->pos.y, C_CHUNK_H) - C_CHUNK_H / 2 - .5f - C_CHUNK_H * y_rel;

                    int chunk_index = chunk_pos_x * map->chunks_h + chunk_pos_y;
                    glBindVertexArray(map->chunks.data[chunk_index].info.vao);
                    glDrawElements(GL_TRIANGLES, map->chunks.data[chunk_index].info.idx_count, GL_UNSIGNED_INT, 0);
                    index++;
                }
            }
        }
    }
    renderer_add(map_floor.info.vao, MI(map_floor, m4_identity()));
    renderer_add(map_ceiling.info.vao, MI(map_ceiling, transform_instance((v3_t){map->w, 1, 0}, (v3_t){0, 0, M_PI}, (v3_t){1, 1, 1})));
    // glBindVertexArray(0);
    // glBindTexture(GL_TEXTURE_2D, 0);

#if 0 //text map
    system("clear");
    for (int i = 0; i < map->h; i++)
    {
        for (int j = 0; j < map->w; j++)
        {
            if (map->data[i * map->w + j] > 20)
                printf("%c", map->data[i * map->w + j]);
            else
                printf("%d", map->data[i * map->w + j]);
        }
        printf("\n");
    }
#endif

#if 1
    //Map
    int offset_x, map_screen_start_x, map_screen_end_x, map_screen_size_x;
    int offset_y, map_screen_start_y, map_screen_end_y, map_screen_size_y;
    offset_x = map_screen_start_x = pos_x - (size_x + .5f) * map->tile_w;
    offset_y = map_screen_start_y = pos_y - (size_y + .5f) * map->tile_h;
    v2_t player_offset = player->pos;
    player_offset.x -= (int)player_offset.x;
    player_offset.y -= (int)player_offset.y;
    const v2_t map_start = {player->pos.x - size_x, player->pos.y - size_y};
    const int map_w = 2 * size_x + 1, map_h = 2 * size_y + 1;

    map_screen_size_x = map_w * map->tile_w;
    map_screen_size_y = map_h * map->tile_h;
    map_screen_end_x = map_screen_start_x + map_screen_size_x;
    map_screen_end_y = map_screen_start_y + map_screen_size_y;

    plane_renderer_add(offset_x + (-(int)player_offset.x + .5) * map->tile_w,
                       offset_y + (-(int)player_offset.y + .5) * map->tile_h,
                       offset_x + (-(int)player_offset.x + 2 * size_x + .5) * map->tile_w,
                       offset_y + (-(int)player_offset.y + 2 * size_y + .5) * map->tile_h, map_floor_texture.id);

    for (int i = 0; i < map_h; i++)
    {
        for (int j = 0; j < map_w; j++)
        {
            const v2_t vx0 = {offset_x + (j - player_offset.x + .5) * map->tile_w,
                              offset_y + (i - player_offset.y + .5) * map->tile_h};
            const v2_t vx1 = {vx0.x + map->tile_w, vx0.y + map->tile_h};
            const float32 map_y = (map_start.y + i);
            const float32 map_x = map_start.x + j;
            if (map_y >= 0 && map_x >= 0 && map_x < map->w && map_y < map->h)
            {
                if (tile_is_wall(map, map_x, map_y))
                {
                    line_renderer_add_quat(vx0.x, vx0.y, map->tile_w, map->tile_h, COLOR(yellow));
                }
            }
            else
            {
                line_renderer_add_quat(vx0.x, vx0.y, map->tile_w, map->tile_h, COLOR(white));
            }
        }
    }

    //Player
    offset_x = pos_x;
    offset_y = pos_y;
    line_renderer_add_circle(offset_x, offset_y, player->hitbox_radius * map->tile_w, COLOR(green));
    line_renderer_add(offset_x,
                      offset_y,
                      offset_x + player->dir.x * (map->tile_w << 1),
                      offset_y + player->dir.y * (map->tile_h << 1), COLOR(green));
    // line_renderer_add(offset_x,
    //                   offset_y,
    //                   offset_x + player->right_frustum.x * (map->tile_w << 4),
    //                   offset_y + player->right_frustum.y * (map->tile_h << 4), COLOR(cyan));
    // line_renderer_add(offset_x,
    //                   offset_y,
    //                   offset_x + player->left_frustum.x * (map->tile_w << 4),
    //                   offset_y + player->left_frustum.y * (map->tile_h << 4), COLOR(cyan));

    //g_screen_draw_circle(VI2TOV2(offset), player->hitbox_radius * map->tile_w, COLOR(green));

    // v2_t hitscan_left,
    //     hitscan_right, hitscan_left_long, hitscan_right_long;
    // hitscan_right = v2_muls((v2_t){player->camera.plane.x, player->camera.plane.z}, player->hitscan_range);
    // hitscan_left = v2_muls(hitscan_right, -1);
    // hitscan_right_long = v2_muls((v2_t){player->camera.plane.x, player->camera.plane.z}, player->hitscan_range);
    // hitscan_left_long = v2_muls(hitscan_right_long, -1);
    // v2_t tmp_dir;
    // tmp_dir = v2_muls((v2_t){player->dir.x, player->dir.y}, 10);
    // hitscan_left_long = v2_add(hitscan_left_long, tmp_dir);
    // hitscan_right_long = v2_add(hitscan_right_long, tmp_dir);
    // line_renderer_add(offset_x + hitscan_left.x * map->tile_w,
    //                   offset_y + hitscan_left.y * map->tile_h,
    //                   offset_x + hitscan_left_long.x * map->tile_w,
    //                   offset_y + hitscan_left_long.y * map->tile_h,
    //                   COLOR(magenta));
    // line_renderer_add(offset_x + hitscan_right.x * map->tile_w,
    //                   offset_y + hitscan_right.y * map->tile_h,
    //                   offset_x + hitscan_right_long.x * map->tile_w,
    //                   offset_y + hitscan_right_long.y * map->tile_h,
    //                   COLOR(magenta));

    //Enemies
    // offset_x = pos_x - player->pos.x * map->tile_w;
    // offset_y = pos_y - player->pos.y * map->tile_h;
    // for (uint i = 0; i < enemies->cap; i++)
    // {
    //     if (enemies->data[i].is_alive)
    //     {
    //         const int rad = enemies->data[i].hitbox_radius * map->tile_w;
    //         v2_t screen_pos = {offset_x + enemies->data[i].pos.x * map->tile_w, offset_y + enemies->data[i].pos.y * map->tile_h};
    //         //debug_v2(offset)
    //         //debug("%d %d", map_screen_pos_x, map_screen_pos_y)
    //         if ((int)screen_pos.x - rad > map_screen_start_x && (int)screen_pos.y - rad > 0 &&
    //             (int)screen_pos.x < (g_scr.w - rad) && (int)screen_pos.y < (map_screen_end_y - rad))
    //         {
    //             color enemy_color;
    //             const EnemyState state = (int)enemies->data[i].state->state;
    //             switch (state)
    //             {
    //             case EnemyState_Follow:
    //                 enemy_color = COLOR(orange);
    //                 break;

    //             case EnemyState_Attack:
    //                 enemy_color = COLOR(red);
    //                 break;

    //             default:
    //                 enemy_color = COLOR(light_gray);
    //                 break;
    //             }
    //             line_renderer_add_circle(screen_pos.x, screen_pos.y, enemies->data[i].hitbox_radius * map->tile_w, COLOR(red));
    //             //line_renderer_add(screen_pos.x,
    //             //                  screen_pos.y,
    //             //                  screen_pos.x + enemies->data[i].hitbox_radius,
    //             //                  screen_pos.y + enemies->data[i].hitbox_radius, COLOR(red));
    //             //g_screen_draw_circle(screen_pos, rad, enemy_color);
    //         }
    //     }
    // }

    //Projectiles
    // offset_x = pos_x - player->pos.x * map->tile_w;
    // offset_y = pos_y - player->pos.y * map->tile_h;
    // for (uint i = 0; i < projectiles->cap; i++)
    // {
    //     if (projectiles->data[i].is_alive)
    //     {
    //         const int rad = projectiles->data[i].radius * map->tile_w;
    //         v2_t screen_pos = {offset_x + projectiles->data[i].pos.x * map->tile_w, offset_y + projectiles->data[i].pos.y * map->tile_h};
    //         if ((int)screen_pos.x - rad > map_screen_start_x && (int)screen_pos.y - rad > 0 &&
    //             (int)screen_pos.x < (g_scr.w - rad) && (int)screen_pos.y < (map_screen_end_y - rad))
    //         {
    //             line_renderer_add_circle(screen_pos.x, screen_pos.y, rad, COLOR(blue));
    //         }
    //     }
    // }

    //Map frame
    // g_screen_fill_quat((v2_t){g_scr.w - map_w * map->tile_w, 0},
    //                    (v2_t){map->tile_w, map_h * map->tile_h},
    //                    COLOR(dark_golden));
    // g_screen_fill_quat((v2_t){g_scr.w - (map_w - 1) * map->tile_w, (map_h - 1) * map->tile_h},
    //                    (v2_t){map_w * map->tile_w, map->tile_h},
    //                    COLOR(dark_golden));
/*
    // HUD
    color player_hp_color_start = COLOR(red);
    color player_hp_color_end = COLOR(green);
    color player_hp_color;
    player_hp_color.r = pi_lerp(player_hp_color_start.r, player_hp_color_end.r, MAX(player->hp / player->max_hp, 0));
    player_hp_color.g = pi_lerp(player_hp_color_start.g, player_hp_color_end.g, MAX(player->hp / player->max_hp, 0));
    player_hp_color.b = pi_lerp(player_hp_color_start.b, player_hp_color_end.b, MAX(player->hp / player->max_hp, 0));
    g_screen_draw_number(5, g_scr.h - 40, player->hp, player_hp_color);
    g_screen_draw_number(115, g_scr.h - 40, enemies->data[0].state_frame, COLOR(green));
    */
#endif
}

void map_clean(Map *map)
{
    free(map->data);
    vec_free(map->chunks);
}
