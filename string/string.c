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
void string_put_char(struct String *string, const char str) {
    RESIZE_ARRAY_IF_NEED(string, 1, char);
    string->length += 1;
    string->list[string->length - 1] = str;
    string->list[string->length] = 0;
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
        string_put_char(X, string->list[i]);
    }
    return X;
}

struct String * string_indent(struct String *string, size_t indent) {
    NEW_STRING(indentData);
    for (size_t i = 0; i < indent; ++i) string_put_char(indentData, ' ');

    NEW_STRING(X);
    bool isNewLine = true;
    for (size_t i = 0; i < string->length; ++i) {
        if (isNewLine) {
            string_put(X, indentData->list);
            isNewLine = false;
            i -= 1;
            continue;
        }
        if (string->list[i] == '\n') {
            string_put(X, "\n");
            isNewLine = true;
            continue;
        }
        string_put_char(X, string->list[i]);
    }
    DESTROY_STRING(indentData);
    return X;
}

// Add string to string array
void string_array_push(struct StringArray *array, char *string) {
    RESIZE_ARRAY_IF_NEED(array, 1, size_t);
    NEW_STRING(str);
    string_put(str, string);
    array->list[array->length] = str;
    array->length++;
}

void string_array_clear(struct StringArray *array) {
    for (size_t i = 0; i < array->length; ++i) {
        ____string_free(array->list[i]);
        // MEMORY_FREE(array->list[i]);
    }
    array->length = 0;
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
struct String *print_string(char *fileName, size_t line, struct String *string, bool writeToBuffer) {
    NEW_STRING(X);
    string_add(X, "String [%zu:%zu] {\n", string->length, string->allocated);
    string_add(X, "    %s'%s'%s\n", ANSI_COLOR_GREEN, string->list, ANSI_COLOR_RESET);
    string_add(X, "}\n");

    LOGGER_LOG(fileName, line, X->list);
    DESTROY_STRING(X);
    return 0;
}

// Print string array
struct String *print_string_array(char *fileName, size_t line, struct StringArray *array, bool writeToBuffer) {
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
    return 0;
}