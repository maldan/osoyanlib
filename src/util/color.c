
#include "../../include/util/color.h"

struct ColorRgbInt hex_to_rgb(char *hex) {
    uint32_t x = (int)strtol(hex, NULL, 16);
    struct ColorRgbInt color;
    color.r = (x >> 16u) & 255u;
    color.g = (x >> 8u) & 255u;
    color.b = x & 255u;
    return color;
}
