#pragma once

#include <wchar.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../include/memory/memory.h"

wchar_t *wchars_substr(const wchar_t *where, ssize_t startIndex, ssize_t toIndex);

char *wchars_to_chars(const wchar_t *where);

size_t wchars_word_length(const wchar_t *word);
