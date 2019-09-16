#pragma once

#include "../osoyan.h"

#define NEW_STRING(X) struct String *X = MEMORY_ALLOCATE_STRUCT(String); ____string_init(X)
#define DESTROY_STRING(X) ____string_free(X)

#define NEW_STRING_ARRAY(X) struct StringArray *X = MEMORY_ALLOCATE_STRUCT(StringArray); ____string_array_init(X)
#define DESTROY_STRING_ARRAY(X) ____string_array_free(X)

struct String {
    DEFAULT_ARRAY_STRUCT(char *)
};

struct StringArray {
    DEFAULT_ARRAY_STRUCT(struct String **)
};

/**
 * Init string
 * @param string - Current string
 */
void ____string_init(struct String *string);

/**
 * Destroy string
 * @param string - Current string
 */
void ____string_free(struct String *string);

/**
 * Init string array
 * @param array - Current string
 */
void ____string_array_init(struct StringArray *array);

/**
 * Destroy string array
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
 * Return result as new %struct String%. Function can take $string$ with multilines not only single lined $string$.
 * Function doesn't works correctly with ansi sequences by now.
 * @param string - Source string
 * @param maxLength - Max length of line
 * @return
 */
struct String * string_break(struct String *string, size_t maxLength);

/**
 * Add $indent$ on each line and return result as new %struct String%.
 * @param string - Source string
 * @param indent - Amount of spaces before each line
 * @return
 */
struct String * string_indent(struct String *string, size_t indent);

/**
 * Almost same as print_chars but also prints some info about $string$ because $string$ is %struct String% not %char%.
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param string - %struct String%, not just %char%
 * @param writeToBuffer - Write to buffer or to screen
 * @return
 */
char* print_string(char *fileName, size_t line, struct String *string, bool writeToBuffer);

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
 * @return
 */
struct String *string_array_join(struct StringArray *array, char *glue);

/**
 * Dispose each string and change length to 0.
 * @param array - Array of strings
 */
void string_array_clear(struct StringArray *array);

/**
 * Print each string that contains into %struct StringArray%. Printed information contain info about size of array and allocated space.
 * Also print length of each string before string.
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param array - Array of strings
 * @param writeToBuffer - Write to buffer or to screen
 * @return
 */
char* print_string_array(char *fileName, size_t line, struct StringArray *array, bool writeToBuffer);