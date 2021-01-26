//
// Created by uberdever on 06.09.2020.
//

#include "game.h"

static SDL_GLContext context = NULL;
uint frames = 0;
int half_w;
int half_h;
bool is_player_exited = false;
bool is_player_dead = false;
bool is_all_enemies_dead = false;
bool is_beginning = true;

Shader shader;
Shader model_shader;
Shader light_shader;
Model map_skybox = {};
Model door = {};
Model medpack = {};
Model bulletpack = {};
Model cellspack = {};
PointLight point_light;
static Texture muzzle_flash;
static Texture map_border_texture = {0};
static Model red_key = {0};
static Model green_key = {0};
static Model blue_key = {0};

static Texture game_over_screen;
static Texture win_screen;
static Texture bad_end_screen;
static Texture intermission_screen;
static Texture beginning_screen;

static Map map = {0};
static Player player = {0};
static v_Enemy_t enemies = {0};
static v_Projectile_t projectiles = {0};
static vec_v3p_t active_doors;

static void gl_init()
{
    v_pConstChar_t samplers_names;
    vec_new(pConstChar_t, samplers_names, 2);
    vec_push(samplers_names, "u_material.diffuse");
    vec_push(samplers_names, "u_material.specular");
    vec_push(samplers_names, "u_material.emission");

    shader = shader_init("graphics/shaders/vs.vs", "graphics/shaders/fs.fs");
    shader_bind(shader);
    shader_find_samplers2D(&shader, samplers_names);
    shader_set_1f(&shader, "u_material.shininess", 12.f);
    vec_free(samplers_names);

    light_shader = shader_init("graphics/shaders/light.vs", "graphics/shaders/light.fs");
    Mesh sphereMesh = generate_sphere(16, 1);
    Model sphere = model_build(sphereMesh);
    renderer_set_dir_light_model(sphere);
    renderer_set_point_light_model(sphere);

    Mesh skyboxMesh = generate_plane((v3_t){0, 0, 0},
                                     (v3_t){1, 0, 0},
                                     (v3_t){1, 0, 1},
                                     (v3_t){0, 0, 1}); //generate_hyperboloid(1, 1, 1, 0, 32);
    map_skybox = model_build(skyboxMesh);
    model_add_texture(&map_skybox, texture_load("graphics/textures/skybox_bright.png"));
    Mesh door_mesh = obj_parse("graphics/models/cube.obj");
    door = model_build(door_mesh);
    model_add_texture(&door, texture_load("graphics/textures/door.png"));
    model_add_texture(&door, texture_load("graphics/textures/door.png"));
    model_add_texture(&door, texture_load("graphics/textures/door_emissive.png"));
    point_light = (PointLight){
        .properties = (Light){
            .ambient = C_P_AMBIENT,
            .diffuse = C_P_DIFFUSE,
            .specular = C_P_SPECULAR,
            .brightness = 1,
            .color = (v3_t){1, 1, 1}},
        .attenuation = C_P_ATTENUATION};

    medpack = voxel_sprite_construct("graphics/sprites/medpack.png",
                                     "graphics/depthmaps/medpack.zaxis",
                                     false,
                                     (v3_t){0.1f, 0.1f, 0.1f});
    model_add_texture(&medpack, texture_load("graphics/sprites/medpack.png"));
    model_add_texture(&medpack, texture_load("graphics/sprites/medpack.png"));
    model_add_texture(&medpack, texture_load("graphics/sprites/medpack_emissive.png"));

    bulletpack = voxel_sprite_construct("graphics/sprites/bulletpack.png",
                                        "graphics/depthmaps/bulletpack.zaxis",
                                        false,
                                        (v3_t){0.06f, 0.06f, 0.06f});
    model_add_texture(&bulletpack, texture_load("graphics/sprites/bulletpack.png"));
    model_add_texture(&bulletpack, texture_load("graphics/sprites/bulletpack.png"));
    model_add_texture(&bulletpack, texture_load("graphics/sprites/bulletpack_emissive.png"));

    cellspack = voxel_sprite_construct("graphics/sprites/cellspack.png",
                                       "graphics/depthmaps/cellspack.zaxis",
                                       false,
                                       (v3_t){0.06f, 0.06f, 0.06f});
    model_add_texture(&cellspack, texture_load("graphics/sprites/cellspack.png"));
    model_add_texture(&cellspack, texture_load("graphics/sprites/cellspack.png"));
    model_add_texture(&cellspack, texture_load("graphics/sprites/cellspack_emissive.png"));

    red_key = voxel_sprite_construct("graphics/sprites/red_keycard.png",
                                     "graphics/depthmaps/red_keycard.zaxis",
                                     false,
                                     (v3_t){0.06f, 0.06f, 0.06f});
    model_add_texture(&red_key, texture_load("graphics/sprites/red_keycard.png"));
    model_add_texture(&red_key, texture_load("graphics/sprites/red_keycard.png"));
    model_add_texture(&red_key, texture_load("graphics/sprites/red_keycard.png"));

    green_key = voxel_sprite_construct("graphics/sprites/green_keycard.png",
                                       "graphics/depthmaps/green_keycard.zaxis",
                                       false,
                                       (v3_t){0.06f, 0.06f, 0.06f});
    model_add_texture(&green_key, texture_load("graphics/sprites/green_keycard.png"));
    model_add_texture(&green_key, texture_load("graphics/sprites/green_keycard.png"));
    model_add_texture(&green_key, texture_load("graphics/sprites/green_keycard.png"));

    blue_key = voxel_sprite_construct("graphics/sprites/blue_keycard.png",
                                      "graphics/depthmaps/blue_keycard.zaxis",
                                      false,
                                      (v3_t){0.06f, 0.06f, 0.06f});
    model_add_texture(&blue_key, texture_load("graphics/sprites/blue_keycard.png"));
    model_add_texture(&blue_key, texture_load("graphics/sprites/blue_keycard.png"));
    model_add_texture(&blue_key, texture_load("graphics/sprites/blue_keycard.png"));

    muzzle_flash = texture_load("graphics/textures/muzzle_flash.png");
    map_border_texture = texture_load("graphics/textures/minimap_border.png");

    game_over_screen = texture_load("graphics/textures/game_over.png");
    win_screen = texture_load("graphics/textures/win.png");
    bad_end_screen = texture_load("graphics/textures/bad_end.png");
    intermission_screen = texture_load("graphics/textures/intermission.png");
    beginning_screen = texture_load("graphics/textures/beginning.png");

    model_shader = shader_init("graphics/shaders/model.vs", "graphics/shaders/model.fs");

    renderer_init();
    line_renderer_init();
    plane_renderer_init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void init(void)
{
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    context = SDL_GL_CreateContext(g_scr.window);

    glewExperimental = GL_TRUE;
    GLenum glew_init = glewInit();

    if (glew_init != GLEW_OK)
        error("Cannot init glew");
    if (!context)
        error("Cannot create OpenGl context");

    debug("OpenGL loaded");
    debug("Vendor:   %s", glGetString(GL_VENDOR));
    debug("Renderer: %s", glGetString(GL_RENDERER));
    debug("Version:  %s", glGetString(GL_VERSION));

    half_w = (g_scr.w) / 2;
    half_h = (g_scr.h) / 2;

    gl_init();

    map_init(&map, "data/huge.map", "graphics/textures/tileset_main.png");
    player_init(&player);
    enemies_init(&enemies, &map);
    projectile_init(&projectiles);

    vec_new(v3p_t, active_doors, 8);

    //enemies_create(&enemies, (v2_t){6, 7}, EnemyTypes_Common);
    //enemies_create(&enemies, (v2_t){3, 12}, EnemyTypes_Strong);
    //enemies_create(&enemies, (v2_t){10, 3.5}, EnemyTypes_Boss);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_CaptureMouse(SDL_TRUE);
}

void begin_frame(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.1f, 0.0f);

    //g_screen_clear();
    //g_screen_buffer_clear();
}

void end_frame(void)
{
    SDL_GL_SwapWindow(g_scr.window);

    //g_screen_buffer_update();
    //g_screen_present();
}

void event_handler(void)
{
    //if (kbd_key_pressed(SDLK_ESCAPE))
    //    is_program_running = false

    player_event(&player);
    if (is_beginning)
    {
        plane_renderer_add(0, 0, g_scr.w, g_scr.h, intermission_screen.id);
        plane_renderer_add(0, 0, g_scr.w, g_scr.h, beginning_screen.id);
        plane_renderer_render();
        end_frame();
    }

    if (mouse_left_down())
    {
        if (!player.cur_weapon->is_recoil && (*player.cur_weapon->bullets) > 0)
        {
            switch (player.cur_weapon->type)
            {
            case WeaponType_Pistol:
            {
                enemies_process_hit(&map, &player, &enemies);
            }
            break;

            case WeaponType_TeslaGun:
            {
                projectile_create(&projectiles, v2_add(player.pos, v2_muls(player.dir, 0.5f)), player.dir);
            }
            break;

            default:
                break;
            }
            player.shots_fired = true;
            player.cur_weapon->is_recoil = true;
            (*player.cur_weapon->bullets)--;
        }

        //
    }
    if (kbd_key_pushed(SDLK_e))
    {
        v2_t action_pos = (v2_t){player.pos.x + player.dir.x, player.pos.y + player.dir.y};
        if (map.data[(int)action_pos.y * map.w + (int)action_pos.x] == '4' - '0')
        {
            if (player.has_red_key)
                player.pos = (v2_t){(int)action_pos.x + .5f, (int)action_pos.y + 1.f * (player.dir.y > 0 ? 1 : -1) + .5f};
        }
        else if (map.data[(int)action_pos.y * map.w + (int)action_pos.x] == '5' - '0')
        {
            if (player.has_green_key)
                player.pos = (v2_t){(int)action_pos.x + 1.f * (player.dir.x > 0 ? 1 : -1) + .5f, (int)action_pos.y + .5f};
        }
        else if (map.data[(int)action_pos.y * map.w + (int)action_pos.x] == '6' - '0')
        {
            if (player.has_blue_key)
                player.pos = (v2_t){(int)action_pos.x + .5f, (int)action_pos.y + 1.f * (player.dir.y > 0 ? 1 : -1) + .5f};
        }
        if (map.data[(int)action_pos.y * map.w + (int)action_pos.x] == 'D')
            map.data[(int)action_pos.y * map.w + (int)action_pos.x] = 0;
        for (int i = 0; i < map.door_positions.size; i++)
        {
            if ((int)action_pos.x + .5f == map.door_positions.data[i].x &&
                (int)action_pos.y + .5f == map.door_positions.data[i].z &&
                map.door_positions.data[i].y < 0.5f)
            {
                bool is_added = false;
                for (int j = 0; j < active_doors.size; j++)
                {
                    if (active_doors.data[j] == NULL)
                    {
                        active_doors.data[j] = &map.door_positions.data[i];
                        is_added = true;
                        break;
                    }
                }
                if (!is_added)
                    vec_push(active_doors, &map.door_positions.data[i]);
            }
        }
    }
}

void update(void)
{

    for (int x = 0; x < map.w; x++)
    {
        for (int y = 0; y < map.h; y++)
        {
            if (!tile_is_wall(&map, x, y) && !tile_is_collectable(&map, x, y))
                map.data[y * map.w + x] = 0;
        }
    }
    for (int i = 0; i < active_doors.size; i++)
    {
        if (active_doors.data[i])
        {
            active_doors.data[i]->y += C_DOOR_SPEED;
            if (active_doors.data[i]->y > 1)
                active_doors.data[i] = NULL;
        }
    }
    for (int i = 0; i < map.door_positions.size; i++)
    {
        if (map.door_positions.data[i].y < 0.5)
        {
            map.data[(int)map.door_positions.data[i].z * map.w + (int)map.door_positions.data[i].x] = 'D';
        }
        else
        {
            map.data[(int)map.door_positions.data[i].z * map.w + (int)map.door_positions.data[i].x] = 0;
        }
    }
    map.data[(int)map.exit_position.z * map.w + (int)map.exit_position.x] = 'E';

    player_update(&map, &player);
    projectile_update(&map, &projectiles);
    enemies_update(&map, &player, &enemies, &projectiles);
}

void render(void)
{
    int map_size_x = 16, map_size_y = 16;
    const int player_screen_pos_x = g_scr.w - (map_size_x * map.tile_w), player_screen_pos_y = map_size_y * map.tile_h;
    player.camera.view_matrix = m4_look_at(player.camera.pos, player.camera.dir, (v3_t){0, 1, 0});

    renderer_bind_shader(&model_shader);
    shader_set_m4(&model_shader, u_projection_name, player.camera.projection_matrix);
    shader_set_m4(&model_shader, u_view_name, player.camera.view_matrix);
    for (int i = 0; i < 5; i++)
    {
        renderer_add(map_skybox.info.vao, MI(map_skybox, transform_instance((v3_t){-16 + i * 32, 6, -4}, (v3_t){M_PI_2, 0, 0}, (v3_t){32, 1, 12})));
    }
    glClear(GL_DEPTH_BUFFER_BIT);
    renderer_render_model(map_skybox);
    renderer_clear_model(map_skybox.info.vao);

    for (int i = 0; i < map.light_positions.size; i++)
    {
        point_light.position = map.light_positions.data[i];
        renderer_add_point_light_to_scene(point_light, MI((Model){0}, transform_instance(point_light.position, (v3_t){0}, (v3_t){.1, .1, .1})));
    }

    //renderer_add_point_light_to_scene(point_light, (ModelInstance){.transform = m4_translation(point_light.position)});

    renderer_bind_light_shader(&light_shader);
    shader_set_m4(&light_shader, u_projection_name, player.camera.projection_matrix);
    shader_set_m4(&light_shader, u_view_name, player.camera.view_matrix);
    renderer_render_lights();

    renderer_bind_shader(&shader);
    for (int i = 0; i < map.light_positions.size; i++)
    {
        point_light.position = map.light_positions.data[i];
        renderer_add_point_light_to_shader(point_light);
    }
    projectile_render(&projectiles);
    //renderer_add_point_light_to_shader(point_light);

    shader_set_m4(&shader, u_projection_name, player.camera.projection_matrix);
    shader_set_m4(&shader, u_view_name, player.camera.view_matrix);
    shader_set_v3(&shader, "u_viewPos", player.camera.pos);
    shader_set_1i(&shader, "u_pLightCount", renderer_get_point_light_count());

    for (int i = 0; i < map.door_positions.size; i++)
    {
        if (map.door_positions.data[i].y > 1)
            continue;
        renderer_add(door.info.vao, MI(door, transform_instance(map.door_positions.data[i], (v3_t){0}, (v3_t){1, 1, 1})));
    }
    for (int i = 0; i < map.medpack_positions.size; i++)
    {
        if (map.medpack_positions.data[i].y > -.4)
            continue;
        renderer_add(medpack.info.vao, MI(medpack, transform_instance(map.medpack_positions.data[i], (v3_t){0}, (v3_t){.1, .1, .1})));
    }
    for (int i = 0; i < map.bullets_positions.size; i++)
    {
        if (map.bullets_positions.data[i].y > -.4)
            continue;
        renderer_add(bulletpack.info.vao, MI(bulletpack, transform_instance(map.bullets_positions.data[i], (v3_t){0}, (v3_t){.1, .1, .1})));
    }
    for (int i = 0; i < map.cells_positions.size; i++)
    {
        if (map.cells_positions.data[i].y > -.4)
            continue;
        renderer_add(cellspack.info.vao, MI(cellspack, transform_instance(map.cells_positions.data[i], (v3_t){0}, (v3_t){.1, .1, .1})));
    }
    if (map.keys_position[0].y <= -.4)
        renderer_add(red_key.info.vao, MI(red_key, transform_instance(map.keys_position[0], (v3_t){M_PI_2}, (v3_t){.1, .1, .1})));
    if (map.keys_position[1].y <= -.4)
        renderer_add(green_key.info.vao, MI(green_key, transform_instance(map.keys_position[1], (v3_t){M_PI_2}, (v3_t){.1, .1, .1})));
    if (map.keys_position[2].y <= -.4)
        renderer_add(blue_key.info.vao, MI(blue_key, transform_instance(map.keys_position[2], (v3_t){M_PI_2}, (v3_t){.1, .1, .1})));

    map_render(&map, &player, &enemies, &projectiles, player_screen_pos_x, player_screen_pos_y, map_size_x, map_size_y);
    enemies_render(&enemies);

    renderer_render();

    renderer_clear_all();

    if (player.shots_fired && player.cur_weapon->type == WeaponType_Pistol)
    {
        plane_renderer_add(0.6f * g_scr.w, 0.5 * g_scr.h, 0.7 * g_scr.w, 0.6 * g_scr.h, muzzle_flash.id);
        plane_renderer_render();
        player.shots_fired = false;
    }
    renderer_bind_shader(&shader);
    player_render(&player); //Render weapon on top of everything else

    if (is_player_exited)
    {
        plane_renderer_add(0, 0, g_scr.w, g_scr.h, intermission_screen.id);
        if (is_all_enemies_dead)
            plane_renderer_add(0, 0, g_scr.w, g_scr.h, win_screen.id);
        else
            plane_renderer_add(0, 0, g_scr.w, g_scr.h, bad_end_screen.id);
    }
    if (is_player_dead)
    {
        plane_renderer_add(0, 0, g_scr.w, g_scr.h, intermission_screen.id);
        plane_renderer_add(0, 0, g_scr.w, g_scr.h, game_over_screen.id);
    }

    if (is_player_dead || is_player_exited)
        glClear(GL_DEPTH_BUFFER_BIT);
    plane_renderer_render();
    if (!is_player_dead && !is_player_exited)
    {

        line_renderer_render();
        const int map_w = 2 * map_size_x + 1, map_h = 2 * map_size_y + 1;
        plane_renderer_add(g_scr.w - (map_w + 1) * map.tile_w, 0, g_scr.w, (map_h + 1) * map.tile_h, map_border_texture.id);
        plane_renderer_render();
        //UI
        glLineWidth(2);
        draw_symbol('+', (v3_t){0}, .03, .06, COLOR(gray));
        draw_symbol((player.has_red_key ? '+' : '-'), (v3_t){.85, -1, 0}, .03, .06, COLOR(red));
        draw_symbol((player.has_green_key ? '+' : '-'), (v3_t){.9, -1, 0}, .03, .06, COLOR(green));
        draw_symbol((player.has_blue_key ? '+' : '-'), (v3_t){.95, -1, 0}, .03, .06, COLOR(blue));
        color player_hp_color_start = COLOR(red);
        color player_hp_color_end = COLOR(green);
        color player_hp_color;
        player_hp_color.r = pi_lerp(player_hp_color_start.r, player_hp_color_end.r, MAX(player.hp / player.max_hp, 0));
        player_hp_color.g = pi_lerp(player_hp_color_start.g, player_hp_color_end.g, MAX(player.hp / player.max_hp, 0));
        player_hp_color.b = pi_lerp(player_hp_color_start.b, player_hp_color_end.b, MAX(player.hp / player.max_hp, 0));
        draw_number(player.hp, 0.05, (v3_t){0, -.1, 0}, .03, .06, player_hp_color);

        draw_number((*player.cur_weapon->bullets), 0.05, (v3_t){0, .1, 0}, .03, .06, COLOR(orange));

        line_renderer_render();
        glLineWidth(1);
    }
    frames++;
    if (frames >= 60)
        frames = 0;
}

void clean(void)
{
    line_renderer_clean();
    plane_renderer_clean();

    player_clean(&player);
    enemies_clean(&enemies);
    projectile_clean(&projectiles);
    map_clean(&map);

    shader_clean(shader);

    SDL_GL_DeleteContext(context);
}