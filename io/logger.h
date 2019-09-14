#pragma once

#include <stddef.h>
#include <stdio.h>
#include "file.h"
#include "console.h"

#define LOGGER_LOG(fileName, line, ...) logger_log(fileName, line, __VA_ARGS__)

void logger_log(const char *fileName, size_t line, const char *format, ...);
