#pragma once

#include "../../osoyan.h"

#define NEW_DICT(X, T) struct Dict *X = MEMORY_ALLOCATE_STRUCT(Dict); ____dict_init(X, #T);
#define EQU_DICT(X) struct Dict *X
#define DESTROY_DICT(X) ____dict_free(X);

struct Dict {
    DEFAULT_ARRAY_STRUCT(void **)
    char **keys;
    const char *type;
};

void ____dict_init(struct Dict *dict, const char *type);

void ____dict_free(struct Dict *dict);

void dict_add(struct Dict *dict, char *key, void *value);

long dict_key_index(struct Dict *dict, char *key);

struct String *print_dict(char *fileName, size_t line, struct Dict *dict, bool writeToBuffer);

/*int dict_get(struct Dict *, char *, void **);

void dict_print(struct Dict *);*/
