#include "../../include/util/args.h"
#include "../../include/memory/memory.h"

struct ArgList *args_init(int amount, char **args) {
    struct ArgList *X = MEMORY_ALLOCATE_STRUCT(ArgList);

    NEW_DICT(Y, char *);
    X->keys = Y;

    NEW_STRING(Y1);
    X->flags = Y1;

    char *currentKey = 0;
    char *tempValue = 0;

    for (int i = 1; i < amount; ++i) {
        // Keys
        if (args[i][0] == '-' && args[i][1] == '-') {
            currentKey = args[i];
            tempValue = MEMORY_ALLOCATE(0);
            dict_add(X->keys, currentKey + 2, tempValue);
        } else if (args[i][0] == '-') {
            for (size_t j = 1; j < strlen(args[i]); ++j)
                string_put_char(X->flags, args[i][j]);
        } else {
            if (!currentKey) {
                printf("Value '%s' not associated with any key", args[i]);
                exit(1);
            }
            MEMORY_FREE(tempValue);
            char *tmp;
            CLONE_CHARS(tmp, args[i]);
            dict_add(X->keys, currentKey + 2, tmp);
            currentKey = 0;
        }
    }

    return X;
}

void args_free(struct ArgList *argList) {
    for (size_t i = 0; i < argList->keys->length; ++i) {
        MEMORY_FREE(argList->keys->list[i]);
    }
    DESTROY_DICT(argList->keys);
    DESTROY_STRING(argList->flags);
    MEMORY_FREE(argList);
}

bool args_has_flags(struct ArgList *argList, char *flags) {
    uint8_t foundFlags = strlen(flags);

    for (size_t i = 0; i < strlen(flags); ++i) {
        for (size_t j = 0; j < argList->flags->length; ++j) {
            if (flags[i] == argList->flags->list[j]) {
                foundFlags--;
                break;
            }
        }
    }

    return foundFlags == 0;
}

bool args_has_key(struct ArgList *argList, char *key) {
    return dict_key_index(argList->keys, key) >= 0;
}

char *args_get_key_value(struct ArgList *argList, char *key) {
    long index = dict_key_index(argList->keys, key);
    if (index < 0) return NULL;
    return argList->keys->list[index];
}
