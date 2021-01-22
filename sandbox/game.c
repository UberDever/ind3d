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

Shader shader;
Shader model_shader;
Shader light_shader;
Model map_skybox = {};
Model door = {};
Model medpack = {};
Model bulletpack = {};
PointLight point_light;
static Texture muzzle_flash;

static Map map;
static Player player;
static v_Enemy_t enemies;
static v_Projectile_t projectiles;
static vec_v3p_t active_doors;

static void gl_init()
{
    v_pConstChar_t samplers_names;
    vec_new(pConstChar_t, samplers_names, 2);
    vec_push(samplers_names, "u_material.diffuse");
    vec_push(samplers_names, "u_material.specular");

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

    bulletpack = voxel_sprite_construct("graphics/sprites/bulletpack.png",
                                        "graphics/depthmaps/bulletpack.zaxis",
                                        false,
                                        (v3_t){0.06f, 0.06f, 0.06f});
    model_add_texture(&bulletpack, texture_load("graphics/sprites/bulletpack.png"));
    model_add_texture(&bulletpack, texture_load("graphics/sprites/bulletpack.png"));

    muzzle_flash = texture_load("graphics/textures/muzzle_flash.png");

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
    //    is_program_running = false;

    player_event(&player);

    if (mouse_left_down())
    {
        if (!player.cur_weapon->is_recoil && player.cur_weapon->bullet_count > 0)
        {
            enemies_process_hit(&map, &player, &enemies);
            player.shots_fired = true;
            player.cur_weapon->is_recoil = true;
            player.cur_weapon->bullet_count--;
        }

        //projectile_create(&projectiles, player.pos, player.dir);
    }
    if (kbd_key_pushed(SDLK_e))
    {
        v2_t action_pos = (v2_t){player.pos.x + player.dir.x, player.pos.y + player.dir.y};
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
    if (kbd_key_pushed(SDLK_c))
    {
        // map.tile_w += 5;
        // map.tile_h += 5;
    }
    if (kbd_key_pushed(SDLK_z))
    {
        // map.tile_w -= 5;
        // map.tile_h -= 5;
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

    map_render(&map, &player, &enemies, &projectiles, player_screen_pos_x, player_screen_pos_y, map_size_x, map_size_y);
    enemies_render(&enemies);

    renderer_render();

    renderer_clear_all();

    if (player.shots_fired)
    {
        plane_renderer_add(0.6f * g_scr.w, 0.5 * g_scr.h, 0.7 * g_scr.w, 0.6 * g_scr.h, muzzle_flash.id);
        plane_renderer_render();
        player.shots_fired = false;
    }
    renderer_bind_shader(&shader);
    player_render(&player); //Render weapon on top of everything else

    plane_renderer_render();
    if (!is_player_dead && !is_player_exited)
    {

        line_renderer_render();

        //UI
        glLineWidth(2);
        draw_symbol('+', (v3_t){0}, .03, .06, COLOR(gray));
        color player_hp_color_start = COLOR(red);
        color player_hp_color_end = COLOR(green);
        color player_hp_color;
        player_hp_color.r = pi_lerp(player_hp_color_start.r, player_hp_color_end.r, MAX(player.hp / player.max_hp, 0));
        player_hp_color.g = pi_lerp(player_hp_color_start.g, player_hp_color_end.g, MAX(player.hp / player.max_hp, 0));
        player_hp_color.b = pi_lerp(player_hp_color_start.b, player_hp_color_end.b, MAX(player.hp / player.max_hp, 0));
        draw_number(player.hp, 0.05, (v3_t){0, -.1, 0}, .03, .06, player_hp_color);

        draw_number(player.cur_weapon->bullet_count, 0.05, (v3_t){0, .1, 0}, .03, .06, COLOR(orange));

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