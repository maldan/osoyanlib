#pragma once

#include <time.h>

#ifdef __MINGW32__
#include <windows.h>
#else
#include <asm/ioctls.h>
#include <sys/ioctl.h>
#endif

#include <libgen.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <libgen.h>
#include <stdarg.h>
#include <limits.h>
#include "../io/file.h"
#include "../io/path.h"
#include "console.h"

/* Standard file descriptors.  */
#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */

#define LOGGER_LOG(fileName, line, ...) logger_log(fileName, line, __VA_ARGS__);
#define LOGGER_HALT(fileName, line, ...) { printf("Error in file %s:%d\n", fileName, line); printf(__VA_ARGS__); exit(1); }

/**
 * Print formatted string on screen with current time and file position where it was call. Usually calls by macros !LOGGER_LOG!
 * @param fileName - Current file name, usually result of macros !__FILE__!
 * @param line - Current line, usually result of macros !__LINE__!
 * @param format - Format for string but also can be just string
 * @param ... - Variadic arguments for $format$
 */
void logger_log(const char *fileName, size_t line, const char *format, ...);
