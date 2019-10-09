#pragma once

#define ANSI_COLOR_RED     "\x1b[91m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_BLUE    "\x1b[94m"
#define ANSI_COLOR_MAGENTA "\x1b[95m"
#define ANSI_COLOR_CYAN    "\x1b[96m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_CLEAR         "\x1b[3J\x1b[H\x1b[2J"
#define ANSI_HIDE_CURSOR         "\x1b[?25l"
#define ANSI_SHOW_CURSOR         "\x1b[?25h"

#define ANSI_BACKGROUND_RED     "\x1b[41m"
#define ANSI_BACKGROUND_GREEN     "\x1b[42m"
#define ANSI_BACKGROUND_YELLOW     "\x1b[43m"
#define ANSI_BACKGROUND_BLUE     "\x1b[44m"

#define ANSI_BOLD "\x1b[1m"

#define	STDIN_FILENO	0
#define	STDOUT_FILENO	1
#define	STDERR_FILENO	2

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __MINGW32__
#include <windows.h>
struct winsize {
    uint16_t ws_col;
    uint16_t ws_row;
};
#else
#include <asm/ioctls.h>
#include <sys/ioctl.h>
#include <termios.h>
extern int isatty (int __fd) __THROW;
#endif

int console_enable_vt_mode();
struct winsize console_get_window_size();
void console_non_canonical_mode();
void console_fill_screen(char);
