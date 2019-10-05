#pragma once

#ifdef __MINGW32__
#else
#include <pwd.h>
#endif

char *os_home_dir(char *addPath);
