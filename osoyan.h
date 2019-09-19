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
#include <libgen.h>
#include <dirent.h>

#define DEFAULT_ARRAY_STRUCT(TYPE) TYPE list;\
size_t length;\
size_t allocated;

#define ARRAY_REMOVE_AT(A, TYPE) size_t len = A->length - (at + 1);\
memmove(A->list + at, A->list + at + amount, len * sizeof(TYPE));\
A->length -= amount

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
char*: print_chars, \
struct String*: print_string, \
struct StringArray*: print_string_array, \
struct Vector*: print_vector, \
struct FileInfo*: print_file_info, \
struct Blob*: print_blob, \
default: print_default)(__FILE__, __LINE__, O, false)

#define PRINT_TO_BUFFER(O) _Generic((O), \
struct FileInfo*: print_file_info, \
default: print_default)(__FILE__, __LINE__, O, true)

#include "include/memory/memory.h"
#include "include/string/string.h"
#include "include/string/base64.h"
#include "include/string/chars.h"
#include "include/io/file.h"
#include "include/debug/logger.h"
#include "include/debug/console.h"
#include "include/crypt/sha1.h"
#include "include/debug/debug.h"
#include "include/list/blob.h"
#include "include/list/vector.h"
//#include "include/list/dict.h"
#include "include/util/args.h"

/**
 * This is default printer for !PRINT! marcos. Just prints pointer of object. For example [0x7fffff00]
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param data - Some pointer
 * @param writeToBuffer - Write to buffer or to screen
 * @return
 */
char * print_default(const char *fileName, size_t line, void *data, bool writeToBuffer);

