#pragma once

#include <stdint.h>
// #include <zconf.h>
#include <stdlib.h>
#include <stdbool.h>

struct ColorRgbInt {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct ColorRgbInt color_hex_to_rgb(char *);
struct ColorRgbInt color_rand_rgb(bool r, bool g, bool b);

char *color_rgb_to_hex(struct ColorRgbInt color);
struct String *color_hex_to_terminal(char *hexColor);
