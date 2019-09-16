#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <regex.h>
#include "../string/string.h"

#define FILE_INFO_INCLUDE_DATA 1

#define NEW_FILE_INFO(X, PATH) struct FileInfo *X = file_get_contents(PATH)
#define DESTROY_FILE_INFO(X) MEMORY_FREE((X)->data); MEMORY_FREE((X)->path); MEMORY_FREE(X)
#define DESTROY_FILE_SEARCH_RESULT(X) for (size_t ii_53534536xcvsdf42g = 0; ii_53534536xcvsdf42g < sas->length; ++ii_53534536xcvsdf42g) {\
DESTROY_FILE_INFO(((struct FileInfo *)sas->list[ii_53534536xcvsdf42g]));\
}\
MEMORY_FREE(sas->list);\
MEMORY_FREE(sas)

#define CURRENT_EXE_LOCATION(X) char *X = MEMORY_ALLOCATE(FILENAME_MAX); readlink("/proc/self/exe", X, FILENAME_MAX)

struct FileInfo {
    char *path;
    char *fileName;
    void *data;
    size_t size;
};

/**
 * Get content of file at specified $path$. Return %struct FileInfo% with info about file such as length, name, path and data.
 * If file not found, function returns 0.
 * @param path - Path of source file
 * @return
 */
struct FileInfo *file_get_contents(const char *path);

/**
 * Put $buffer$ into a file at specified $path$. It will overwrite the previous file. File will be created if it doesnt't exist.
 * @param path - Path of destination file
 * @param buffer - Buffer with data, can be string or byte array
 * @param length - Length of content, if $length$ is 0 then length will be result of strlen($buffer$)
 * @return
 */
bool file_put_contents(const char *path, void *buffer, size_t length);

/**
 * Recursively search files with regex $filter$ at specified $path$. If $filter$ is !NULL! return all files.
 * Returns %struct Vector% of %struct FileInfo%.
 * @param path - Path of directory to search files
 * @param filter - Regex for filter
 * @param flags - 0 return files, !FILE_INFO_INCLUDE_DATA! include content to files
 * @return
 */
struct Vector *file_search(const char *path, const char *filter, size_t flags);

/**
 * Print info from $file$ struct about file such as size, name and path.
 * @param fileName - Current file name. It's not file which we want to print, it's usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param file - Struct with file info
 * @param writeToBuffer - Write to buffer or to screen
 * @return
 */
struct String *print_file_info(const char *fileName, size_t line, struct FileInfo *file, bool writeToBuffer);