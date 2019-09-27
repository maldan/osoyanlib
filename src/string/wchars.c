#include "../../include/string/wchars.h"

size_t wchars_word_length(const wchar_t *word) {
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

wchar_t *wchars_substr(const wchar_t *where, ssize_t startIndex, ssize_t toIndex) {
    size_t len = (toIndex - startIndex) * sizeof(wchar_t);
    wchar_t *sas = MEMORY_ALLOCATE(len + sizeof(wchar_t));
    MEMORY_COPY(sas, where + startIndex, len, sas, len);
    return sas;
}

char *wchars_to_chars(const wchar_t *where) {
    size_t len = wcslen(where);
    char *out = MEMORY_ALLOCATE(len + 1);
    for (size_t i = 0; i < len; ++i) out[i] = where[i];
    return out;
}
