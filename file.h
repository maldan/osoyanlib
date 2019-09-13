#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "string.h"

#define CURRENT_EXE_LOCATION(X) char *X = calloc(1, FILENAME_MAX); readlink("/proc/self/exe", X, FILENAME_MAX); X = file_get_path(X)

int file_get_contents(const char *path, char **buffer, size_t *length);
int file_put_contents(const char *path, char *buffer, size_t length);
char *file_get_path(char *fullPath);
char *file_get_filename(char *fullPath);