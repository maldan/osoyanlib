#pragma once

#include <stddef.h>
#include <stdio.h>
#include "file.h"
#include "console.h"

#define LOGGER_LOG(fileName, line, ...) logger_log(fileName, line, __VA_ARGS__)
#define LOGGER_HALT(fileName, line, ...) printf("Error in file %s:%d\n", fileName, line); printf(__VA_ARGS__); exit(1)

void logger_log(const char *fileName, size_t line, const char *format, ...);