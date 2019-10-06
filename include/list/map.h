#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "../memory/memory.h"
#include "../string/chars.h"
#include "../string/string.h"

#define NEW_MAP(X, T) struct Map *X = MEMORY_ALLOCATE_STRUCT(Map); ____map_init(X, #T);
#define INIT_MAP(X, T) X = MEMORY_ALLOCATE_STRUCT(Map); ____map_init(X, #T);
#define EQU_MAP(X) struct Map *X
#define DESTROY_MAP(X) ____map_free(X);

struct Map {
    DEFAULT_ARRAY_STRUCT(void **)
    char **keys;
    const char *type;
};

void ____map_init(struct Map *map, const char *type);

void ____map_free(struct Map *map);

void map_add(struct Map *map, char *key, void *value);

ssize_t map_key_index(struct Map *map, char *key);

void *map_get(struct Map *map, char *key);


