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

/**
 * Init vector
 * @param vector - Current vector
 * @param type - Type of vector, it's need only for correct printing
 */
void ____vector_init(struct Vector *vector, const char *type);

/**
 * Destroy vector, but it destroy only list, not elements. Because each element is just reference or number.
 * @param vector - Current vector
 */
void ____vector_free(struct Vector *vector);

/**
 * Add reference $value$ to the end of $vector$
 * @param vector - Current vector
 * @param value - Reference to some value
 */
void vector_push(struct Vector *vector, void *value);

/**
 * Get value in $vector$ at specified $position$. If value not found it returns 0.
 * @param vector - Current vector
 * @param position - Position in vector
 * @return
 */
void *vector_get(struct Vector *vector, size_t position);

/**
 * Print elements of $vector$. Printing values depends on type of $vector$. Usually it prints only pointers to elements.
 * By now it also can prints %char%, %struct String% and %struct FileInfo%. Information also contains type, length and allocated length of $vector$
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param vector - Vector
 * @param writeToBuffer - Write to buffer or to screen
 * @return
 */
char* print_vector(char *fileName, size_t line, struct Vector *vector, bool writeToBuffer);

/*int vector_get(struct Vector *vector, size_t index, void **out);
void vector_push(struct Vector *vector, void *value);
void vector_remove_at(struct Vector *vector, size_t at, size_t amount);
void vector_insert_at(struct Vector *vector, size_t at, void *value);*/

// void vector_print(struct Vector *vector, char *format);