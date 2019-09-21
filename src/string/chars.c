#include "../../include/string/chars.h"
#include "../../include/debug/logger.h"

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

char *chars_substr(const char *where, ssize_t from, ssize_t to) {
    size_t len = to - from;
    char *sas = MEMORY_ALLOCATE(len + 1);
    MEMORY_COPY(sas, where + from, len, sas, len);
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

char *chars_clone(char *src) {
    size_t len = strlen(src);
    char *clone = MEMORY_ALLOCATE(len + 1);
    MEMORY_COPY(clone, src, len, clone, len + 1)
    return clone;
}

struct String *print_chars(const char *fileName, size_t line, void *data, bool writeToBuffer) {
    LOGGER_LOG(fileName, line, "%s", data)
    return 0;
}
