//
// Created by uberdever on 14.08.2020.
//
// Very simple text output, for debug purposes
#include "../include/gamma.h"

#define LET_SCR_W 20
#define LET_SCR_H 35

static const uint letter_offset = (LET_SCR_W * 3) / 2;

static const bool alphabet[][7][5] = {0,1,1,1,0,1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,0,1,1,1,0, // 0
                                      0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1, // 1
                                      1,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1, // 2
                                      1,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0, // 3
                                      1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1, // 4
                                      0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0, // 5
                                      0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0, // 6
                                      1,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0, // 7
                                      0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0, // 8
                                      0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0, // 9
                                      0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1, // A
                                      1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0, // B
                                      0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,1,0, // C
                                      1,1,1,0,0,1,0,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,0,0, // D
                                      0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,1,0,1,1,1,1, // E
                                      1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0, // F
                                      0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,1,1,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0, // G
                                      1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1, // H
                                      1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1, // I
                                      1,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0, // J
                                      1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1, // K
                                      1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,0, // L
                                      1,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1, // M
                                      1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1, // N
                                      0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0, // O
                                      1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0, // P
                                      0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,0,1,0,0,1,1,0,1, // Q
                                      1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1, // R
                                      0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0, // S
                                      1,1,1,1,1,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0, // T
                                      1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0, // U
                                      1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0, // V
                                      1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0,1, // W
                                      1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1, // X
                                      1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0, // Y
                                      1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,1, // Z
};


void g_screen_draw_symbol(uint x, uint y, uint8 symbol, color color) {
    for (uint i = 0; i < LET_SCR_W; i++)
    {
        for (uint j = 0; j < LET_SCR_H; j++)
        {
            const uint x_ = (i + x);
            const uint y_ = (j + y);
            if (pi_aabb_box_x_point_by_size(x_, y_, 0, 0, g_scr.w, g_scr.h))
                g_scr.screen_buf[y_ * g_scr.w + x_] = alphabet[symbol][(j / 5)][(i / 4)] ? color.as_uint : 0 ;
        }
    }
}

void g_screen_draw_number(uint x, uint y, uint number, color color)
{
    if (number == 0)
    {
        g_screen_draw_symbol(x, y, 0, color);
        return;
    }
    const uint digit_count = pi_log10(number);
    for (uint i = 0; number; i++)
    {
        g_screen_draw_symbol(x + ((digit_count - i) * letter_offset), y, number % 10, color);
        number /= 10;
    }
}

void g_screen_draw_string(uint x, uint y, const char *string, color color) {
    uint char_index = 0;
    for (const char* c = string; *c; c++, char_index++)
    {
        if (*c >= '0' && *c <= '9')
        {
            g_screen_draw_symbol(x + char_index * letter_offset, y, (*c - 48), color);
        }
        else if (*c >= 'A' && *c <= 'Z')
        {
            g_screen_draw_symbol(x + char_index * letter_offset, y, (*c - 55), color);
        } else if (*c >= 'a' && *c <= 'z')
        {
            g_screen_draw_symbol(x + char_index * letter_offset, y, (*c - 87), color);
        }
    }
}