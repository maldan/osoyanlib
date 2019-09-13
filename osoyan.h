#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdint.h>
#include <wchar.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

#define RESIZE_ARRAY_IF_NEED(A, S) if (A->length + S > A->allocated) {\
A->list = MEMORY_REALLOCATE(A->list, A->allocated * 2 * sizeof(size_t));\
A->allocated *= 2;\
} 1

#define INIT_DEFAULT_LIST_SIZE(A) A->list = MEMORY_ALLOCATE_AT(fileName, function, line, sizeof(size_t) * 4);\
A->length = 0;\
A->allocated = 4

#define PRINT(O) _Generic((O), \
char*: chars_print, \
struct String*: print_string, \
struct StringArray*: print_string_array, \
default: default_print)(#O, O)

void chars_print(char *, void *);
void default_print(char *, void *);

#include "memory.h"
#include "file.h"