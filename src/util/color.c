#include "../../include/util/color.h"
#include "../../include/string/string.h"

struct ColorRgbInt color_hex_to_rgb(char *hex) {
    uint32_t x = (int)strtol(hex, NULL, 16);
    struct ColorRgbInt color;
    color.r = (x >> 16u) & 255u;
    color.g = (x >> 8u) & 255u;
    color.b = x & 255u;
    return color;
}

struct ColorRgbInt color_rand_rgb(bool r, bool g, bool b) {
    struct ColorRgbInt color;
    color.r = r ?rand() :0;
    color.g = g ?rand() :0;
    color.b = b ?rand() :0;
    return color;
}

char *color_rgb_to_hex(struct ColorRgbInt color) {
    NEW_STRING(X)
    string_add(X, "#%02X%02X%02X", color.r, color.g, color.b);
    char *str = X->list;
    MEMORY_FREE(X);
    return str;
}

struct String *color_hex_to_terminal(char *hexColor) {
    NEW_STRING(out)

    if (hexColor[0] == '#') {
        struct ColorRgbInt x = color_hex_to_rgb(hexColor + 1);
        string_add(out, "\x1b[38;2;%d;%d;%dm", x.r, x.g, x.b);
    }

    return out;
}
