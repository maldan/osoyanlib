#pragma once

#include "../../include/string/string.h"

#ifdef __MINGW32__
#include "windows.h"
#else
#include <pwd.h>
#include <unistd.h>
#endif

char *os_home_dir(char *addPath);
void os_sleep(int ms);
