#include "chars.h"
#include "../io/logger.h"

int chars_to_int(const char *str) {
    return (int)chars_to_long(str);
}

long chars_to_long(const char *str) {
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

char *chars_substr(const char *where, long from, long to) {
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

char* print_chars(const char *fileName, size_t line, void *data, bool writeToBuffer) {
    LOGGER_LOG(fileName, line, "%s", data);
    return 0;
}