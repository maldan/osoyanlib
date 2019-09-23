#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*#define CLONE_CHARS(DST, SRC, V) char *V = MEMORY_ALLOCATE(strlen(SRC) + 1);\
memcpy(V, SRC, strlen(SRC));\
(DST) = V*/

#define CLONE_CHARS(DST, SRC) (DST) = MEMORY_ALLOCATE(strlen(SRC) + 1);\
memcpy((DST), SRC, strlen(SRC))

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

/**
 * Just print chars
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param data - Chars array
 * @param writeToBuffer - Write to buffer or to screen
 * @return Returns %struct String *% if $writeToBuffer$ is !true!
 */
struct String *print_chars(const char *fileName, size_t line, void *data, bool writeToBuffer);
