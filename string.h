#pragma once

#include "osoyan.h"

#define NEW_STRING(X) struct String *X = MEMORY_ALLOCATE_AT(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, sizeof(struct String)); ____string_init(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, X)
#define VAR_STRING(X) struct String X; ____string_init(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, &X)
#define DESTROY_STRING(X) ____string_free(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, &X)
#define VAR_STRING_ARRAY(X) struct StringArray X; ____string_array_init(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, &X)
#define DESTROY_STRING_ARRAY(X) ____string_array_free(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, &X)

struct String {
    char *list;
    size_t length;
    size_t allocated;
};

struct StringArray {
    struct String **list;
    size_t length;
    size_t allocated;
};

void ____string_init(char *fileName, char *function, size_t line, struct String *string);
void ____string_free(char *fileName, char *function, size_t line, struct String *string);
void ____string_array_init(char *fileName, char *function, size_t line, struct StringArray *array);
void ____string_array_free(char *fileName, char *function, size_t line, struct StringArray *array);

/// String
void string_put(struct String *string, const char *str);
struct StringArray string_split(char *string, const char *delimiter, size_t maxAmount);
void print_string(char *varName, struct String *string);

/// String array
void string_array_push(struct StringArray *array, char *);
void print_string_array(char *varName, struct StringArray *array);