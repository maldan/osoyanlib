#include <stddef.h>

struct ArgList {
    size_t amount;
};

void args_init(int amount, char **args);
