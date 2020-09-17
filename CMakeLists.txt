cmake_minimum_required(VERSION 3.16)
project(ind3d C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(
        src
        include

        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

add_executable(${PROJECT_NAME} sandbox/main.c include/alpha.h include/delta.h src/alpha.c src/alphabeta.c include/alphabeta.h include/delta/vec.h include/gamma.h src/gamma.c include/pi.h include/omega.h include/beta.h include/delta/hash_table.h include/delta/queue.h include/delta/stack.h include/delta/deck.h src/gamma/alphabet.c sandbox/main.c sandbox/game.h sandbox/game.c sandbox/util.h sandbox/game/map.h sandbox/game/map.c sandbox/game/player.h sandbox/game/player.c sandbox/game/projectile.h sandbox/game/projectile.c sandbox/game/enemy.h sandbox/game/enemy.c)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        -lm)