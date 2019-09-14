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
#include <stdarg.h>

#include "memory/memory.h"
#include "string/string.h"
#include "string/base64.h"
#include "string/chars.h"
#include "io/file.h"
#include "io/logger.h"

#define RESIZE_ARRAY_IF_NEED(A, S, EL_SIZE) if (A->length + S > A->allocated) {\
size_t _minToAdd34cdf33435_Sdfsd4x = (A->length + S) - A->allocated; \
A->list = MEMORY_REALLOCATE(A->list, _minToAdd34cdf33435_Sdfsd4x + A->allocated * 2 * sizeof(EL_SIZE));\
A->allocated *= 2;\
A->allocated += _minToAdd34cdf33435_Sdfsd4x;\
} 1

#define INIT_DEFAULT_LIST_SIZE(A, TYPE) A->list = MEMORY_ALLOCATE(sizeof(TYPE) * 4);\
A->length = 0;\
A->allocated = 4

#define PRINT(O) _Generic((O), \
char*: chars_print, \
struct String*: print_string, \
struct StringArray*: print_string_array, \
struct FileInfo*: print_file_info, \
default: default_print)(__FILE__, __LINE__, O)

void default_print(const char *, size_t line, void *);

