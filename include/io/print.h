#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "../debug/logger.h"
#include "../list/blob.h"
#include "../list/dict.h"
#include "../list/map.h"
#include "../list/vector.h"
#include "../util/number.h"
#include "../string/chars.h"
#include "../string/string.h"
#include "../string/wstring.h"
#include "../geom/rect.h"

#define PRINT(O) _Generic((O), \
char*: print_chars, \
int: print_int, \
bool: print_bool, \
int16_t: print_int16, \
size_t: print_size_t, \
struct String*: print_string, \
struct WString*: print_wstring, \
struct StringArray*: print_string_array, \
struct Vector*: print_vector, \
struct FileInfo*: print_file_info, \
struct Blob*: print_blob, \
struct Dict*: print_dict, \
struct Rectangle*: print_rect, \
default: print_default)(__FILE__, __LINE__, O, false);

#define PRINT_TO_BUFFER(O) _Generic((O), \
struct FileInfo*: print_file_info, \
default: print_default)(__FILE__, __LINE__, O, true)

/**
 * This is default printer for !PRINT! marcos. Just prints pointer of object. For example [0x7fffff00]
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param data - Some pointer
 * @param writeToBuffer - Write to buffer or to screen
 * @return
 */
char * print_default(const char *fileName, size_t line, void *data, bool writeToBuffer);

/**
 * Print info from $file$ struct about file such as size, name and path.
 * @param fileName - Current file name. It's not file which we want to print, it's usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param file - Struct with file info
 * @param writeToBuffer - Write to buffer or to screen
 * @return Returns %struct String *% if $writeToBuffer$ is !true!
 */
struct String *print_file_info(const char *fileName, size_t line, struct FileInfo *file, bool writeToBuffer);

/**
 * Prints byte array as hex string. For example FF FF 00 3F
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param blob - Array of bytes
 * @param writeToBuffer - Write to buffer or to screen
 * @return Returns %struct String *% if $writeToBuffer$ is !true!
 */
struct String * print_blob(char *fileName, size_t line, struct Blob *blob, bool writeToBuffer);

struct String *print_dict(char *fileName, size_t line, struct Dict *dict, bool writeToBuffer);

struct String *print_map(char *fileName, size_t line, struct Map *map, bool writeToBuffer);

/**
 * Print elements of $vector$. Printing values depends on type of $vector$. Usually it prints only pointers to elements.
 * By now it also can prints %char%, %struct String% and %struct FileInfo%. Information also contains type, length and allocated length of $vector$
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param vector - Vector
 * @param writeToBuffer - Write to buffer or to screen
 * @return Returns %struct String *% if $writeToBuffer$ is !true!
 */
struct String * print_vector(char *fileName, size_t line, struct Vector *vector, bool writeToBuffer);

/**
 * Just print chars
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param data - Chars array
 * @param writeToBuffer - Write to buffer or to screen
 * @return Returns %struct String *% if $writeToBuffer$ is !true!
 */
struct String *print_chars(const char *fileName, size_t line, void *data, bool writeToBuffer);

/**
 * Almost same as print_chars but also prints some info about $string$ because $string$ is %struct String% not %char%.
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param string - %struct String%, not just %char%
 * @param writeToBuffer - Write to buffer or to screen
 * @return Returns %struct String *% if $writeToBuffer$ is !true!
 */
struct String *print_string(char *fileName, size_t line, struct String *string, bool writeToBuffer);

struct String *print_wstring(char *fileName, size_t line, struct WString *wstring, bool writeToBuffer);

/**
 * Print each string that contains into %struct StringArray%. Printed information contain info about size of array and allocated space.
 * Also print length of each string before string.
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param array - Array of strings
 * @param writeToBuffer - Write to buffer or to screen
 * @return Returns %struct String *% if $writeToBuffer$ is !true!
 */
struct String *print_string_array(char *fileName, size_t line, struct StringArray *array, bool writeToBuffer);

struct String *print_size_t(const char *fileName, size_t line, size_t data, bool writeToBuffer);

struct String *print_int(const char *fileName, size_t line, int data, bool writeToBuffer);

struct String *print_int16(const char *fileName, size_t line, int16_t data, bool writeToBuffer);

struct String *print_rect(const char *fileName, size_t line, struct Rectangle *rect, bool writeToBuffer);

struct String *print_bool(const char *fileName, size_t line, bool val, bool writeToBuffer);

void print_as_blob(char *array, size_t length);
