#include "../../include/string/chars.h"
#include <math.h>
#include <sys/param.h>

int chars_to_int(const char *str) {
    return (int)chars_to_long(str);
}

ssize_t chars_to_long(const char *str) {
    char num[48];

    // Copy valid part of number
    for (int i = 0; i < 48; ++i) {
        num[i] = str[i];

        // Minus must be 1st
        if (str[i] == '-' && i > 0) {
            num[i] = 0;
            break;
        }

        // If not digit
        if (str[i] < '0' || str[i] > '9') {
            num[i] = 0;
            break;
        }
    }

    return atol(num);
}

char *chars_substr(const char *where, ssize_t startIndex, ssize_t toIndex) {
    size_t len = toIndex - startIndex;
    char *sas = MEMORY_ALLOCATE(len + 1);
    MEMORY_COPY(sas, where + startIndex, len, sas, len);
    return sas;
}

long chars_index_of(const char *where, const char *index) {
    size_t len = strlen(where);
    size_t indexId = 0;
    size_t indexLen = strlen(index);

    for (size_t i = 0; i < len; ++i) {
        if (where[i] == index[indexId++]) {
            if (indexId == indexLen) {
                return (long)(i + 1) - (long)indexLen;
            }
        } else {
            indexId = 0;
        }
    }

    return -1;
}

// Split string into chars array
struct StringArray * chars_split(char *string, const char *delimiter, size_t maxAmount) {
    NEW_STRING_ARRAY(out);

    size_t strLen = strlen(string);
    size_t delimiterLen = strlen(delimiter);
    size_t delimiterId = 0;
    size_t tempLength = 0;
    size_t stringLastPosition = 0;
    size_t tupleAmount = maxAmount;
    bool isStop = false;

    // Go through all characters
    for (size_t i = 0; i < strLen; ++i) {
        tempLength++;
        if (isStop) continue;

        // Str == delimiter
        if (string[i] == delimiter[delimiterId++]) {
            // Full match
            if (delimiterId == delimiterLen) {
                char *temp = MEMORY_ALLOCATE(tempLength - delimiterLen + 1);
                MEMORY_COPY(temp, string + stringLastPosition, tempLength - delimiterLen, temp, tempLength - delimiterLen);
                string_array_push(out, temp);
                MEMORY_FREE(temp);

                stringLastPosition += tempLength;
                tempLength = 0;
                delimiterId = 0;

                if (maxAmount > 0) {
                    tupleAmount--;
                    if (tupleAmount <= 0)
                        isStop = true;
                }
            }
        } else {
            delimiterId = 0;
        }
    }

    if (tempLength > 0) {
        char *temp = MEMORY_ALLOCATE(tempLength + 1);
        MEMORY_COPY(temp, string + stringLastPosition, tempLength, temp, tempLength);
        string_array_push(out, temp);
        MEMORY_FREE(temp);
    }

    return out;
}

size_t chars_word_length(const char *word) {
    size_t amount = 0;
    size_t pos = 0;

    while (true) {
        if (word[pos] == ' '
        || word[pos] == '\0' || word[pos] == '\n'
        || word[pos] == '\r') break;
        pos++;
        amount++;
    }
    return amount;
}

size_t chars_utf8_length(const char *str) {
    size_t count = 0;
    while (*str)
        count += ((uint8_t)(*str++) & 0xC0u) != 0x80;
    return count;
}

char *chars_clone(char *src) {
    size_t len = strlen(src);
    char *clone = MEMORY_ALLOCATE(len + 1);
    MEMORY_COPY(clone, src, len, clone, len + 1);
    return clone;
}

void chars_set(char *dst, char *src, size_t dstMax) {
    size_t len = strlen(src);
    if (dstMax > 0) {
        memcpy(dst, src, MIN(len, dstMax));
    } else memcpy(dst, src, len);
}

char *chars_replace(char *src, char *pattern, char *replace) {
    size_t len = strlen(src);
    size_t patternLen = strlen(pattern);
    size_t delimiterId = 0;

    NEW_STRING(out)
    NEW_STRING(buffer)

    for (size_t i = 0; i < len; ++i) {
        string_put_char(buffer, src[i]);

        if (src[i] == pattern[delimiterId++]) {
            if (delimiterId == patternLen) {
                delimiterId = 0;
                string_clear(buffer);
                string_put(out, replace);
                continue;
            }
        } else {
            string_put(out, buffer->list);
            string_clear(buffer);
            delimiterId = 0;
        }
    }

    DESTROY_STRING(buffer)
    char *outStr = out->list;
    MEMORY_FREE(out);

    return outStr;
}

bool chars_match(char *src, char *pattern, size_t flags) {
    // Compile regex
    regex_t filterRegex;
    int reti = regcomp(&filterRegex, pattern, REG_EXTENDED | flags);
    if (reti) return false;

    int reti2 = regexec(&filterRegex, src, 0, NULL, 0);
    // Free regex
    regfree(&filterRegex);

    if (reti2) return false;
    return true;
}

size_t chars_char_amount(char *src, char chr) {
    size_t len = strlen(src);
    size_t out = 0;
    for (size_t i = 0; i < len; ++i)
        if (src[i] == chr) out++;
    return out;
}
