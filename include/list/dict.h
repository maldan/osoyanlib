#pragma once

#include "../../osoyan.h"

#define NEW_DICT(X, T) struct Dict *X = MEMORY_ALLOCATE_STRUCT(Dict); ____dict_init(X, #T);
#define INIT_DICT(X, T) X = MEMORY_ALLOCATE_STRUCT(Dict); ____dict_init(X, #T);
#define EQU_DICT(X) struct Dict *X
/*#define SET_DICT(X, Y) { X = MEMORY_ALLOCATE_STRUCT(Dict); ____dict_init(X, "char *");\
for (size_t dict_init_idx228 = 0; dict_init_idx228 < sizeof(Y) / sizeof(Y[0]); ++dict_init_idx228) {\
dict_add(X, Y[dict_init_idx228][0], chars_clone(Y[dict_init_idx228][1]));\
} }*/
#define DESTROY_DICT(X) ____dict_free(X);
#define DESTROY_DICT_FULLY(X) { for (size_t dict_dstr_idx228 = 0; dict_dstr_idx228 < X->length; ++dict_dstr_idx228) {\
MEMORY_FREE(X->list[dict_dstr_idx228]);\
}\
____dict_free(X); }

struct Dict {
    DEFAULT_ARRAY_STRUCT(void **)
    char **keys;
    const char *type;
};

void ____dict_init(struct Dict *dict, const char *type);

void ____dict_free(struct Dict *dict);

void dict_add(struct Dict *dict, char *key, void *value);

void *dict_get(struct Dict *dict, char *key);

ssize_t dict_key_index(struct Dict *dict, char *key);

bool dict_has_key(struct Dict *dict, char *key);

struct String *print_dict(char *fileName, size_t line, struct Dict *dict, bool writeToBuffer);

/*int dict_get(struct Dict *, char *, void **);

void dict_print(struct Dict *);*/
