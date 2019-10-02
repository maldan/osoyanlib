#include <netinet/in.h>
#include "../../include/string/wstring.h"

void ____wstring_init(struct WString *wstring) {
    INIT_DEFAULT_LIST_SIZE(wstring, wchar_t);
}

void ____wstring_free(struct WString *wstring) {
    MEMORY_FREE(wstring->list);
    MEMORY_FREE(wstring);
}

char wstring_char_at(struct WString *wstring, size_t position) {
    return 0;
}

void wstring_put(struct WString *wstring, const char *chars) {
    size_t len = chars_utf8_length(chars);
    RESIZE_ARRAY_IF_NEED(wstring, (len + 1), wchar_t)

    size_t i = 0;
    uint8_t *text = (uint8_t *) chars;
    for (size_t n = 0; n < len; n++) {
        if ((text[i] & 0x80u) == 0) {
            // 1 byte code point, ASCII
            wstring->list[n + wstring->length] = (text[i] & 0x7Fu);
            i += 1;
        } else if ((text[i] & 0xE0u) == 0xC0u) {
            // 2 byte code point
            wstring->list[n + wstring->length] = (text[i] & 0x1Fu) << 6u | (text[i + 1] & 0x3Fu);
            i += 2;
        } else if ((text[i] & 0xF0u) == 0xE0u) {
            // 3 byte code point
            wstring->list[n + wstring->length] =
                    (text[i] & 0xFu) << 12u | (text[i + 1] & 0x3Fu) << 6u | (text[i + 2] & 0x3Fu);
            i += 3;
        } else {
            // 4 byte code point
            wstring->list[n + wstring->length] =
                    (text[i] & 0x7u) << 18u | (text[i + 1] & 0x3Fu) << 12u | (text[i + 2] & 0x3Fu) << 6u |
                    (text[i + 3] & 0x3Fu);
            i += 4;
        }
    }

    // wstring
    wstring->length += len;
    wstring->list[wstring->length] = 0;
}

void wstring_put_wchar(struct WString *wstring, wchar_t chr) {
    RESIZE_ARRAY_IF_NEED(wstring, 2, wchar_t)
    wstring->length += 1;
    wstring->list[wstring->length - 1] = chr;
    wstring->list[wstring->length] = 0;
}

void wstring_clear(struct WString *wstring) {
    wstring->list[0] = 0;
    wstring->length = 0;
}
