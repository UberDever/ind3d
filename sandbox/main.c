//
// Created by uberdever on 06.09.2020.
//

#include "game.h"
#include <sys/time.h>
suseconds_t framerate = 0;
int main()
{
    alphabeta_init((gamma_info_t){1280, 720, true, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL, 0});

    const uint frameskip = 1000 / 60;
    uint time = SDL_GetTicks();

    events_set_handler(event_handler);
    init();
    while (is_program_running)
    {
        if (time + frameskip <= SDL_GetTicks())
        {
            struct timeval tv0;
            gettimeofday(&tv0, NULL);

            suseconds_t timeS = tv0.tv_usec;

            time = SDL_GetTicks();
            begin_frame();
            events_process();
            while ((is_player_exited || is_player_dead) && is_program_running)
            {
                events_process();
            }
            update();
            render();
            end_frame();

            struct timeval tv1;
            gettimeofday(&tv1, NULL);
            framerate = tv1.tv_usec - tv0.tv_usec;
            //debug("%f", 10e6 / (float)framerate);
        }
    }
    //clean();
    alphabeta_clean();
    return 0;
}