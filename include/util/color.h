#pragma once

#include <stdint.h>
#include <zconf.h>
#include <stdlib.h>

struct ColorRgbInt {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct ColorRgbInt hex_to_rgb(char *);
