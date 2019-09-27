#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "../../include/list/list.h"
#include "../../include/memory/memory.h"
#include "../../include/string/string.h"

#define NEW_DICT(X) struct Dict *X = MEMORY_ALLOCATE_STRUCT(Dict); ____dict_init(X);
#define INIT_DICT(X) X = MEMORY_ALLOCATE_STRUCT(Dict); ____dict_init(X);
#define EQU_DICT(X) struct Dict *X
#define DESTROY_DICT(X) ____dict_free(X);

struct Dict {
    DEFAULT_ARRAY_STRUCT(char **)
    char **keys;
};

void ____dict_init(struct Dict *dict);

void ____dict_free(struct Dict *dict);

void dict_add(struct Dict *dict, char *key, char *value);

char *dict_get(struct Dict *dict, char *key);

int dict_get_int(struct Dict *dict, char *key);

bool dict_value_is(struct Dict *dict, char *key, char *value);

ssize_t dict_key_index(struct Dict *dict, char *key);

bool dict_has_key(struct Dict *dict, char *key);


