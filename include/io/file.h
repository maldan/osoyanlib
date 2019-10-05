#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <regex.h>
#include <dirent.h>
#include <libgen.h>

#include "../memory/memory.h"
#include "../string/string.h"
#include "../string/chars.h"
#include "../list/blob.h"
#include "../list/vector.h"
#include "../debug/logger.h"

#define FILE_INFO_INCLUDE_DATA 1

#define NEW_FILE_INFO(X, PATH) struct FileInfo *X = file_get_info(PATH, false);
#define DESTROY_FILE_INFO(X) MEMORY_FREE((X)->data); MEMORY_FREE((X)->path); MEMORY_FREE(X)
#define DESTROY_FILE_SEARCH_RESULT(X) for (size_t ii_53534536xcvsdf42g = 0; ii_53534536xcvsdf42g < X->length; ++ii_53534536xcvsdf42g) {\
DESTROY_FILE_INFO(((struct FileInfo *)X->list[ii_53534536xcvsdf42g]));\
}\
MEMORY_FREE(X->list);\
MEMORY_FREE(X)

#define CURRENT_EXE_LOCATION(X) char *X = MEMORY_ALLOCATE(FILENAME_MAX); readlink("/proc/self/exe", X, FILENAME_MAX)

struct FileInfo {
    char *path;
    char *fileName;
    void *data;
    size_t size;
};

struct FileInfo *file_get_info(const char *path, bool includeContent);

struct Blob *file_get_slice_of_contents(const char *path, size_t start, size_t length);

struct Blob *file_get_contents(const char *path);

size_t file_get_size(const char *path);

/**
 * Put $buffer$ into a file at specified $path$. It will overwrite the previous file. File will be created if it doesnt't exist.
 * @param path - Path of destination file
 * @param buffer - Buffer with data, can be string or byte array
 * @param length - Length of content, if $length$ is 0 then length will be result of strlen($buffer$)
 * @return Return !true! if success otherwise returns !false!.
 */
bool file_put_contents(const char *path, void *buffer, size_t length);

bool file_put_blob(const char *path, struct Blob *blob);

/**
 * Recursively search files with regex $filter$ at specified $path$.
 * @param path - Path of directory to search files
 * @param filter - Regex for filter
 * @param flags - 0 return files, !FILE_INFO_INCLUDE_DATA! include content to files
 * @return Returns %struct Vector% with %struct FileInfo% elements. Return all files if $filter$ is !NULL!.
 */
struct Vector *file_search(const char *path, const char *filter, size_t flags);

bool file_copy(char *fromPath, char *toPath);
