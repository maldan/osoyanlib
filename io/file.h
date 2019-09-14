#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../string/string.h"

#define NEW_FILE_INFO(X, PATH) struct FileInfo *X = file_get_contents(PATH)
#define DESTROY_FILE_INFO(X) MEMORY_FREE(X->data); MEMORY_FREE(X)
#define CURRENT_EXE_LOCATION(X) char *X = MEMORY_ALLOCATE(FILENAME_MAX); readlink("/proc/self/exe", X, FILENAME_MAX)

struct FileInfo {
    const char *path;
    char *fileName;
    void *data;
    size_t size;
};

struct FileInfo * file_get_contents(const char *path);
bool file_put_contents(const char *path, char *buffer, size_t length);
void print_file_info(const char *fileName, size_t line, struct FileInfo *data);