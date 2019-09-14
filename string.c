#include "string.h"

void ____string_init(struct String *string) {
    //MEMORY_IN;
    INIT_DEFAULT_LIST_SIZE(string, char);
    //MEMORY_OUT;
}

void ____string_free(char *fileName, char *function, size_t line, struct String *string) {
    // MEMORY_FREE_AT(fileName, function, line, string->list);
    MEMORY_FREE(string->list);
}

void ____string_array_init(struct StringArray *array) {
    //MEMORY_IN;
    INIT_DEFAULT_LIST_SIZE(array, struct String *);
    //MEMORY_OUT;
}

void ____string_array_free(char *fileName, char *function, size_t line, struct StringArray *array) {
    // Free all strings
    for (size_t i = 0; i < array->length; ++i) {
        ____string_free(fileName, function, line, array->list[i]);
        MEMORY_FREE(array->list[i]);
        //MEMORY_FREE_AT(fileName, function, line, array->list[i]);
    }
    MEMORY_FREE(array->list);
    //MEMORY_FREE_AT(fileName, function, line, array->list);
}

void print_string(char *varName, struct String *string) {
    printf("char[%zu] '%s'", string->length, string->list);
}

void print_string_array(char *varName, struct StringArray *array) {
    size_t totalSize = 0;
    for (size_t j = 0; j < array->length; ++j)
        totalSize += array->list[j]->allocated;

    printf("StringArray %s [%zu] {\n", varName, totalSize);
    for (size_t i = 0; i < array->length; ++i) {
        printf("    ");
        PRINT(array->list[i]);
        if (i < array->length - 1) printf(",\n");
    }
    printf("\n}\n");
}

void string_put(struct String *string, const char *str) {
    //MEMORY_IN;
    size_t len = strlen(str);
    RESIZE_ARRAY_IF_NEED(string, len + 1, char);
    MEMORY_COPY(string->list + string->length, str, len, string->list, string->allocated);
    string->length += len;
    string->list[string->length] = 0;
    //MEMORY_OUT;
}

void string_add(struct String *string, char *format, ...) {
    //MEMORY_IN;
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
    //MEMORY_OUT;
}

struct StringArray string_split(char *string, const char *delimiter, size_t maxAmount) {
    //MEMORY_IN;
    VAR_STRING_ARRAY(out);
    //MEMORY_OUT;

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
                //MEMORY_IN;
                string_array_push(&out, temp);
                //MEMORY_OUT;
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
        string_array_push(&out, temp);
        MEMORY_FREE(temp);
    }


    return out;
}

void string_array_push(struct StringArray *array, char *string) {
    //MEMORY_IN;
    RESIZE_ARRAY_IF_NEED(array, 1, size_t);
    NEW_STRING(str);
    string_put(str, string);
    array->list[array->length] = str;
    array->length++;
    //MEMORY_OUT;
}

struct String *string_array_join(struct StringArray *array, char *glue) {
    NEW_STRING(str);
    /*for (size_t i = 0; i < array->length; ++i) {
        string_put(str, array->list[i]->list);
        if (i < array->length - 1) string_put(str, glue);
    }*/
    return str;
}