#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdint.h>
#include <wchar.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdarg.h>
#include <libgen.h>
#include <dirent.h>

#ifdef __MINGW32__
#else
#include <wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <sys/stat.h>
#include <fcntl.h>
#include <fcntl.h>
#include <errno.h>

#include "include/memory/memory.h"
#include "include/string/string.h"
#include "include/string/base64.h"
#include "include/string/chars.h"
#include "include/io/file.h"
#include "include/io/print.h"
#include "include/debug/logger.h"
#include "include/debug/console.h"
#include "include/crypt/sha1.h"
#include "include/debug/debug.h"
#include "include/list/blob.h"
#include "include/list/vector.h"
#include "include/list/dict.h"
#include "include/list/map.h"
#include "include/list/bitset.h"
#include "include/util/args.h"
#include "include/util/color.h"
#include "include/util/number.h"
#include "include/util/os.h"
#include "include/net/http.h"
#include "include/net/websocket.h"
#include "include/ui/termui.h"
#include "include/compress/huffman.h"



