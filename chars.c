#include "chars.h"

int chars_to_int(const char *str) {
    return (int)chars_to_long(str);
}

long chars_to_long(const char *str) {
    char num[48];

    // Copy valid part of number
    for (int i = 0; i < 48; ++i) {
        num[i] = str[i];

        // Minus must be 1st
        if (str[i] == '-' && i > 0) {
            num[i] = 0;
            break;
        }

        // If not digit
        if (str[i] < '0' || str[i] > '9') {
            num[i] = 0;
            break;
        }
    }

    return atol(num);
}

void chars_print(char *s, void *v) {
    printf("%s", (char *)v);
}