#pragma once

#include "../osoyan.h"

#define NEW_STRING(X) struct String *X = MEMORY_ALLOCATE(sizeof(struct String)); ____string_init(X)
#define DESTROY_STRING(X) ____string_free(X)

#define NEW_STRING_ARRAY(X) struct StringArray *X = MEMORY_ALLOCATE(sizeof(struct StringArray)); ____string_array_init(X)
#define DESTROY_STRING_ARRAY(X) ____string_array_free(X)

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

void ____string_init(struct String *string);
void ____string_free(struct String *string);
void ____string_array_init(struct StringArray *array);
void ____string_array_free(struct StringArray *array);

/// String
void string_put(struct String *string, const char *str);
void string_add(struct String *string, char *format, ...);
struct StringArray * string_split(char *string, const char *delimiter, size_t maxAmount);
void print_string(char *fileName, size_t line, struct String *string);

/// String array
void string_array_push(struct StringArray *array, char *);
struct String *string_array_join(struct StringArray *array, char *);
void print_string_array(char *fileName, size_t line, struct StringArray *array);