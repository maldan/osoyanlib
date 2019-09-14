#include "string.h"

void ____string_init(struct String *string) {
    INIT_DEFAULT_LIST_SIZE(string, char);
}
void ____string_array_init(struct StringArray *array) {
    INIT_DEFAULT_LIST_SIZE(array, struct String *);
}
void ____string_free(struct String *string) {
    MEMORY_FREE(string->list);
    MEMORY_FREE(string);
}
void ____string_array_free(struct StringArray *array) {
    // Free all strings
    for (size_t i = 0; i < array->length; ++i)
        ____string_free(array->list[i]);
    MEMORY_FREE(array->list);
    MEMORY_FREE(array);
}

// Just concatenate string with chars
void string_put(struct String *string, const char *str) {
    size_t len = strlen(str);
    RESIZE_ARRAY_IF_NEED(string, len + 1, char);
    MEMORY_COPY(string->list + string->length, str, len, string->list, string->allocated);
    string->length += len;
    string->list[string->length] = 0;
}

// Concatenation with format
void string_add(struct String *string, char *format, ...) {
    char *str;
    va_list argPtr;
    va_start(argPtr, format);

    // Measure string length
    size_t length = vsnprintf(NULL, 0, format, argPtr);
    str = MEMORY_ALLOCATE(length + 1);

    // Add formatted string
    va_start(argPtr, format);
    vsprintf(str, format, argPtr);
    string_put(string, str);
    MEMORY_FREE(str);
    va_end(argPtr);
}

// Break long lines
struct String * string_break(struct String *string, size_t maxLength) {
    NEW_STRING(X);
    size_t counter = 0;
    for (size_t i = 0; i < string->length; ++i) {
        if (string->list[i] == '\n') {
            string_put(X, "\n");
            counter = 0;
            continue;
        }
        if (counter++ > maxLength) {
            string_put(X, "\n");
            counter = 0;
            i -= 1;
            continue;
        }
        string_add(X, "%c", string->list[i]);
    }
    return X;
}

// Split string into chars array
struct StringArray * string_split(char *string, const char *delimiter, size_t maxAmount) {
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

// Add string to string array
void string_array_push(struct StringArray *array, char *string) {
    RESIZE_ARRAY_IF_NEED(array, 1, size_t);
    NEW_STRING(str);
    string_put(str, string);
    array->list[array->length] = str;
    array->length++;
}

// Join string array into single string
struct String *string_array_join(struct StringArray *array, char *glue) {
    NEW_STRING(str);
    for (size_t i = 0; i < array->length; ++i) {
        string_put(str, array->list[i]->list);
        if (i < array->length - 1) string_put(str, glue);
    }
    return str;
}

void string_array_remove_at(struct StringArray *array, size_t at, size_t amount) {
    ARRAY_REMOVE_AT(array, size_t);
}

// Print string
void print_string(char *fileName, size_t line, struct String *string) {
    NEW_STRING(X);
    string_add(X, "String [%zu:%zu] {\n", string->length, string->allocated);
    string_add(X, "    %s'%s'%s\n", ANSI_COLOR_GREEN, string->list, ANSI_COLOR_RESET);
    string_add(X, "}\n");

    LOGGER_LOG(fileName, line, X->list);
    DESTROY_STRING(X);
}

// Print string array
void print_string_array(char *fileName, size_t line, struct StringArray *array) {
    size_t totalSize = 0;
    for (size_t j = 0; j < array->length; ++j)
        totalSize += array->list[j]->allocated;

    NEW_STRING(X);

    string_add(X, "StringArray [%zu:%zu] {\n", array->length, totalSize);
    for (size_t i = 0; i < array->length; ++i) {
        string_add(X, "    (%s%zu%s)%s'%s'%s", ANSI_COLOR_RED, array->list[i]->length, ANSI_COLOR_RESET, ANSI_COLOR_GREEN, array->list[i]->list, ANSI_COLOR_RESET);
        if (i < array->length - 1) string_add(X, ",\n");
    }
    string_add(X, "\n}\n");

    LOGGER_LOG(fileName, line, X->list);
    DESTROY_STRING(X);
}