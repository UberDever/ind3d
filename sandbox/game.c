//
// Created by uberdever on 06.09.2020.
//

#include "game.h"

static SDL_GLContext context = NULL;
uint frames = 0;
int half_w;
int half_h;

static Map map;
static Player player;
static v_Enemy_t enemies;
static v_Projectile_t projectiles;

void init(void)
{
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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

    map_init(&map, "../../data/huge.map", "../../graphics/textures/tileset_cardinal.png", "../../graphics/shaders/map.vs", "../../graphics/shaders/map.fs");
    player_init(&player);
    enemy_init(&enemies);
    projectile_init(&projectiles);

    enemy_create(&enemies, (v2_t){6, 7}, EnemyTypes_Common);
    //enemy_create((v2){3, 12}, EnemyTypes_Strong);
    //enemy_create((v2){10, 3.5}, EnemyTypes_Boss);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    //SDL_CaptureMouse(SDL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glFrontFace(GL_CW);
    glFrontFace(GL_CCW);
}

void begin_frame(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

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
    if (kbd_key_pressed(SDLK_ESCAPE))
        is_program_running = false;

    player_event(&player);

    if (mouse_left_down())
    {
        enemies_process_hit(&map, &player, &enemies);
        //projectile_create(&projectiles, player.pos, player.dir);
    }
    if (kbd_key_pushed(SDLK_c))
    {
        map.tile_w += 5;
        map.tile_h += 5;
    }
    if (kbd_key_pushed(SDLK_z))
    {
        map.tile_w -= 5;
        map.tile_h -= 5;
    }
}

void update(void)
{
    for (int x = 0; x < map.w; x++)
    {
        for (int y = 0; y < map.h; y++)
        {
            if (!tile_is_wall(&map, x, y))
                map.data[y * map.w + x] = 0;
        }
    }
    player_update(&map, &player);
    projectile_update(&map, &projectiles);
    enemy_update(&map, &player, &enemies, &projectiles);
}

void render(void)
{
    int map_size_x = 16, map_size_y = 16;
    const int player_screen_pos_x = g_scr.w - (map_size_x * map.tile_w), player_screen_pos_y = map_size_y * map.tile_h;
    //player_raycast(&map, &player);
    map_render(&map, &player, &enemies, &projectiles, player_screen_pos_x, player_screen_pos_y, map_size_x, map_size_y);

    //g_screen_draw_number(5, 5, frames, COLOR(yellow));
    frames++;
    if (frames >= 60)
        frames = 0;
}

void clean(void)
{
    player_clean(&player);
    enemy_clean(&enemies);
    projectile_clean(&projectiles);
    map_clean(&map);

    SDL_GL_DeleteContext(context);
}