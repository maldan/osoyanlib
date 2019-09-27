#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../../include/debug/logger.h"

/*#define CLONE_CHARS(DST, SRC, V) char *V = MEMORY_ALLOCATE(strlen(SRC) + 1);\
memcpy(V, SRC, strlen(SRC));\
(DST) = V*/

#define CLONE_CHARS(DST, SRC) (DST) = MEMORY_ALLOCATE(strlen(SRC) + 1);\
memcpy((DST), SRC, strlen(SRC))

#define HEX_TO_INT(X) (int)strtol(X, NULL, 16);

/**
 * Convert first corrects chars to %int%
 * @param source - Source chars array
 * @return Returns int
 */
int chars_to_int(const char *source);

/**
 * Convert first corrects chars to %long%
 * @param source - Source chars array
 * @return Returns long
 */
long chars_to_long(const char *source);

/**
 * Get index of $pattern$
 * @param where - Source chars array
 * @param pattern - Search pattern
 * @return Returns index of found $pattern$. Returns -1 if $pattern$ not found.
 */
ssize_t chars_index_of(const char *where, const char *pattern);

char *chars_substr(const char *where, ssize_t startIndex, ssize_t toIndex);


size_t chars_word_length(const char *word);


size_t chars_utf8_length(const char *str);

/**
 * Split chars array $string$ into pieces by $delimiter$. The result will be stored into new %struct StringArray%.
 * Function also take $maxAmount$ argument. If it 0 then function will split to the end of the $string$.
 * If it 1 then function split only once, for example "1-2-3" will return ["1", "2-3"].
 * @param string
 * @param delimiter
 * @param maxAmount
 * @return Return %struct StringArray% that contains all splits strings
 */
struct StringArray * chars_split(char *string, const char *delimiter, size_t maxAmount);

char *chars_clone(char *src);

