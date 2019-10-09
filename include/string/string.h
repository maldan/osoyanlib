/**
 * This file contains functions for working with strings and string arrays. By strings means %struct String% not %char%. For %char% there is another file #chars.h#.
 * %struct String% is like dynamic array for %char%. It contains fields like $list$, $length$ and $allocated$.
 * You can add new chars to string and it will be dynamically resized by need.
 * There is also %struct StringArray% and functions for it. String array is like vector of %struct String% but there is differences.
 * For example vector contains only reference to real object. But string array contain references to a copy of real object. So it can be safely disposed later.
 */

#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#include "../list/list.h"
#include "../memory/memory.h"

#define EQU_STRING(X) struct String *X
#define INIT_STRING(X, V) (X) = MEMORY_ALLOCATE_STRUCT(String); ____string_init(X); string_put(X, V);
#define NEW_STRING(X) struct String *X = MEMORY_ALLOCATE_STRUCT(String); ____string_init(X);
#define DESTROY_STRING(X) ____string_free(X);

#define EQU_STRING_ARRAY(X) struct StringArray *X
#define NEW_STRING_ARRAY(X) struct StringArray *X = MEMORY_ALLOCATE_STRUCT(StringArray); ____string_array_init(X);
#define DESTROY_STRING_ARRAY(X) ____string_array_free(X);

struct String {
    DEFAULT_ARRAY_STRUCT(char *)
};

struct StringArray {
    DEFAULT_ARRAY_STRUCT(struct String **)
};

/**
 * Init %struct String%. Don't use this function directly, instead use macros !NEW_STRING!
 * @param string - Current string
 */
void ____string_init(struct String *string);

/**
 * Destroy %struct String%. Don't use this function directly, instead use macros !DESTROY_STRING!
 * @param string - Current string
 */
void ____string_free(struct String *string);

/**
 * Init %struct StringArray%. Don't use this function directly, instead use macros !NEW_STRING_ARRAY!
 * @param array - Current string
 */
void ____string_array_init(struct StringArray *array);

/**
 * Destroy %struct StringArray%. Don't use this function directly, instead use macros !DESTROY_STRING_ARRAY!
 * @param array - Current string
 */
void ____string_array_free(struct StringArray *array);

/**
 * Just add single character $chr$ to the end of the $string$.
 * @param string - Current string
 * @param chr - Char you want to add to the string
 */
void string_put_char(struct String *string, const char chr);

/**
 * Just add $chars$ to the end of $string$.
 * @param string - Current string
 * @param chars - Chars you want to add to the string
 */
void string_put(struct String *string, const char *chars);

/**
 * Add formatted string into $string$. It's like ~sprintf~ to $string$
 * @param string - Source string
 * @param format - Format for string but also can be just string
 * @param ... - Variadic argument using in $format$
 */
void string_add(struct String *string, char *format, ...);

/**
 * Break $string$ (or just wrap $string$ on new line) if it contains more than $maxLength$ chars on line.
 * Function can take $string$ with multilines not only single lined $string$.
 * Function doesn't works correctly with ansi sequences by now.
 * @param string - Source string
 * @param maxLength - Max length of line
 * @return Return formatted result as new %struct String%.
 */
struct String *string_break(struct String *string, size_t maxLength);

/**
 * Add $indent$ on each line.
 * @param string - Source string
 * @param indent - Amount of spaces before each line
 * @return Return formatted result as new %struct String%.
 */
struct String *string_indent(struct String *string, size_t indent);

void string_clear(struct String *string);

/**
 * Add $string$ into $array$. Note that function make copy of $string$ not just reference it.
 * @param array - Array of strings
 * @param string - Array of chars that will be copied and added to $array$
 */
void string_array_push(struct StringArray *array, char *string);

/**
 * Join strings in $array$ into %struct String% with $glue$.
 * For example if array contain strings ["1", "2", "3"] and glue is "-" then we will get %struct String% that contains "1-2-3" result.
 * @param array - Array of strings
 * @param glue - Contain chars that will be between strings. Also can be !NULL!
 * @return Returns %struct String% that contains concatenated strings
 */
struct String *string_array_join(struct StringArray *array, char *glue);

void string_array_remove_at(struct StringArray *array, ssize_t at, size_t amount);

/**
 * Dispose each string and change length to 0.
 * @param array - Array of strings
 */
void string_array_clear(struct StringArray *array);
