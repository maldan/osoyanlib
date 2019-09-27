#include "../../include/io/print.h"

char * print_default(const char *fileName, size_t line, void *data, bool writeToBuffer) {
    LOGGER_LOG(fileName, line, "[%p]", data);
    return 0;
}

struct String* print_file_info(const char *fileName, size_t line, struct FileInfo *data, bool writeToBuffer) {
    NEW_STRING(X);

    string_add(X, "FileInfo {\n");
    /*string_add(X, "    path: %s\"%s\"%s,\n", ANSI_COLOR_GREEN, data->path, ANSI_COLOR_RESET);
    string_add(X, "    fileName: %s\"%s\"%s,\n", ANSI_COLOR_GREEN, data->fileName, ANSI_COLOR_RESET);
    string_add(X, "    size: %s%zu%s\n",  ANSI_COLOR_RED, data->size, ANSI_COLOR_RESET);*/
    string_add(X, "    path: \"%s\",\n", data->path);
    string_add(X, "    fileName: \"%s\",\n", data->fileName);
    string_add(X, "    size: %zu\n", data->size);
    string_add(X, "}\n");

    if (!writeToBuffer) {
        LOGGER_LOG(fileName, line, X->list);
        DESTROY_STRING(X);
    }

    return X;
}

struct String * print_blob(char *fileName, size_t line, struct Blob *blob, bool writeToBuffer) {
    NEW_STRING(X);
    size_t counter = 0;
    for (size_t i = 0; i < blob->length; ++i) {
        string_add(X, "%.2X ", blob->list[i]);
        if (counter++ > 12) {
            counter = 0;
            string_add(X, "\n");
        }
    }
    string_add(X, "\n");
    LOGGER_LOG(fileName, line, X->list);
    DESTROY_STRING(X);
    return 0;
}

struct String *print_dict(char *fileName, size_t line, struct Dict *dict, bool writeToBuffer) {
    NEW_STRING(X);

    string_add(X, "Dict [%zu:%zu] {\n", dict->length, dict->allocated);
    for (size_t i = 0; i < dict->length; ++i) {
        string_add(X, "    \"%s\" => \"%s\"", dict->keys[i], dict->list[i]);
        if (i < dict->length - 1) string_add(X, ",\n");
    }
    string_add(X, "\n}\n");

    LOGGER_LOG(fileName, line, X->list);
    DESTROY_STRING(X);
    return 0;
}

struct String *print_map(char *fileName, size_t line, struct Map *map, bool writeToBuffer) {
    NEW_STRING(X);

    string_add(X, "Map <%s> [%zu:%zu] {\n", map->type, map->length, map->allocated);
    for (size_t i = 0; i < map->length; ++i) {
        string_add(X, "    \"%s\" => %p", map->keys[i], map->list[i]);
        if (i < map->length - 1) string_add(X, ",\n");
    }
    string_add(X, "\n}\n");

    LOGGER_LOG(fileName, line, X->list);
    DESTROY_STRING(X);
    return 0;
}

// Print string array
struct String * print_vector(char *fileName, size_t line, struct Vector *vector, bool writeToBuffer) {
    NEW_STRING(X);

    string_add(X, "Vector <%s> [%zu:%zu] {\n", vector->type, vector->length, vector->allocated);
    for (size_t i = 0; i < vector->length; ++i) {
        if (strcasecmp(vector->type, "struct FileInfo") == 0) {
            struct String *str = print_file_info(fileName, line, vector->list[i], true);
            str->list[str->length - 1] = 0;
            struct String *strWithIndent = string_indent(str, 4);
            string_add(X, "%s", strWithIndent->list);
            DESTROY_STRING(str);
            DESTROY_STRING(strWithIndent);
        } else
        if (strcasecmp(vector->type, "struct String") == 0) {
            string_add(X, "    (%zu)%s'%s'%s",
                       ((struct String *) vector->list[i])->length, ANSI_COLOR_GREEN,
                       ((struct String *) vector->list[i])->list, ANSI_COLOR_RESET);
        } else if (strcasecmp(vector->type, "char") == 0) {
            string_add(X, "    %s'%s'%s", ANSI_COLOR_GREEN, vector->list[i], ANSI_COLOR_RESET);
        } else
            string_add(X, "    %s[%p]%s", ANSI_COLOR_RED, vector->list[i], ANSI_COLOR_RESET);
        if (i < vector->length - 1) string_add(X, ",\n");
    }
    string_add(X, "\n}\n");

    LOGGER_LOG(fileName, line, X->list);
    DESTROY_STRING(X);
    return 0;
}

struct String *print_chars(const char *fileName, size_t line, void *data, bool writeToBuffer) {
    LOGGER_LOG(fileName, line, "%s", data)
    return 0;
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

struct String *print_wstring(char *fileName, size_t line, struct WString *wstring, bool writeToBuffer) {
    NEW_STRING(X);
    string_add(X, "WString [%zu:%zu] {\n", wstring->length, wstring->allocated);
    string_add(X, "    %s'%ls'%s\n", ANSI_COLOR_GREEN, wstring->list, ANSI_COLOR_RESET);
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

struct String *print_size_t(const char *fileName, size_t line, size_t data, bool writeToBuffer) {
    LOGGER_LOG(fileName, line, "(size_t) %zu", data)
    return 0;
}

struct String *print_int(const char *fileName, size_t line, int data, bool writeToBuffer) {
    LOGGER_LOG(fileName, line, "(int) %d", data)
    return 0;
}

struct String *print_int16(const char *fileName, size_t line, int16_t data, bool writeToBuffer) {
    LOGGER_LOG(fileName, line, "(int16_t) %d", data)
    return 0;
}

void print_as_blob(char *array, size_t length) {
    NEW_STRING(X);
    size_t counter = 0;
    for (size_t i = 0; i < length; ++i) {
        string_add(X, "%.2X ", (uint8_t)*(array + i));
        if (counter++ > 12) {
            counter = 0;
            string_add(X, "\n");
        }
    }

    LOGGER_LOG(__FILE__, __LINE__, X->list);
    DESTROY_STRING(X);
}
