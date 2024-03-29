#pragma once

#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "../../include/memory/memory.h"
#include "../list/blob.h"

/**
 * Hash $str$ by sha1 algorithm.
 * @param str - Source string
 * @return Return byte array %struct Blob% with 20 bytes of representation the hash.
 */
struct Blob * sha1_chars(char *str);
