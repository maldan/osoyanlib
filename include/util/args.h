#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "../memory/memory.h"
#include "../list/dict.h"

#define EQU_ARGS(X) struct ArgList *X

struct ArgList {
    size_t amount;
    struct String *flags;
    struct Dict *keys;
};

struct ArgList *args_init(int amount, char **args);

void args_free(struct ArgList *argList);

bool args_has_flags(struct ArgList *argList, char *flags);

bool args_has_key(struct ArgList *argList, char *key);

char *args_get_key_value(struct ArgList *argList, char *key);
