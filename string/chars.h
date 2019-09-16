#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define CLONE_CHARS(DST, SRC, V) char *V = MEMORY_ALLOCATE(strlen(SRC) + 1);\
memcpy(V, SRC, strlen(SRC));\
(DST) = V

/**
 * Convert first corrects chars to %int%
 * @param source - Source chars array
 * @return
 */
int chars_to_int(const char *source);

/**
 * Convert first corrects chars to %long%
 * @param source - Source chars array
 * @return
 */
long chars_to_long(const char *source);

/**
 * Returns index of found $pattern$. Returns -1 if $pattern$ not found.
 * @param where - Source chars array
 * @param pattern - Search pattern
 * @return
 */
long chars_index_of(const char *where, const char *pattern);

/**
 * Returns slice of chars with specified $length$ starting from $startIndex$. Returns "" if slice is incorrect.
 * Function trim slice end if $length$ is out of range.
 * @param where - Source chars array
 * @param startIndex - Start index of source chars array
 * @param length - Length of slice
 * @return
 */
char *chars_substr(const char *where, long startIndex, long length);

/**
 * Split chars array $string$ into pieces by $delimiter$. The result will be stored into new %struct StringArray%.
 * Function also take $maxAmount$ argument. If it 0 then function will split to the end of the $string$.
 * If it 1 then function split only once, for example "1-2-3" will return ["1", "2-3"].
 * @param string
 * @param delimiter
 * @param maxAmount
 * @return
 */
struct StringArray * chars_split(char *string, const char *delimiter, size_t maxAmount);

/**
 * Just print chars
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param data - Chars array
 * @param writeToBuffer - Write to buffer or to screen
 * @return
 */
char *print_chars(const char *fileName, size_t line, void *data, bool writeToBuffer);