#pragma once

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../osoyan.h"

#define NEW_VECTOR(X, T) struct Vector *X = MEMORY_ALLOCATE_STRUCT(Vector); ____vector_init(X, #T)
#define DESTROY_VECTOR(X) ____vector_free(X)

struct Vector {
    DEFAULT_ARRAY_STRUCT(void**);
    const char *type;
};

void ____vector_init(struct Vector *vector, const char *type);
void ____vector_free(struct Vector *vector);

void vector_push(struct Vector *vector, void *value);
void *vector_get(struct Vector *vector, size_t position);
char* print_vector(char *fileName, size_t line, struct Vector *vector, bool writeToBuffer);

/*int vector_get(struct Vector *vector, size_t index, void **out);
void vector_push(struct Vector *vector, void *value);
void vector_remove_at(struct Vector *vector, size_t at, size_t amount);
void vector_insert_at(struct Vector *vector, size_t at, void *value);*/

// void vector_print(struct Vector *vector, char *format);