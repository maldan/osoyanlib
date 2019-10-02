#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../list/list.h"
#include "../memory/memory.h"

#define EQU_WSTRING(X) struct WString *X
#define INIT_WSTRING(X, V) (X) = MEMORY_ALLOCATE_STRUCT(WString); ____wstring_init(X); wstring_put(X, V);
#define NEW_WSTRING(X) struct WString *X = MEMORY_ALLOCATE_STRUCT(WString); ____wstring_init(X);
#define DESTROY_WSTRING(X) ____wstring_free(X);

struct WString {
    DEFAULT_ARRAY_STRUCT(wchar_t *)
};

void ____wstring_init(struct WString *wstring);

void ____wstring_free(struct WString *wstring);

char wstring_char_at(struct WString *wstring, size_t position);

void wstring_put(struct WString *wstring, const char *chars);

void wstring_put_wchar(struct WString *wstring, wchar_t chr);

void wstring_clear(struct WString *wstring);
