#pragma once

#include <stdint.h>
#include <string.h>
#include <stdint.h>

/**
 * Encrypt $str$ by sha1 algorithm and return result as byte array %struct Blob%
 * @param str - Source string
 * @return
 */
struct Blob * sha1_chars(char *str);