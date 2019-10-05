#include "../../include/list/dict.h"

void ____dict_init(struct Dict *dict) {
    dict->list = MEMORY_ALLOCATE(sizeof(char **) * 4);
    dict->keys = MEMORY_ALLOCATE(sizeof(char **) * 4);
    dict->length = 0;
    dict->allocated = 4;
}

void ____dict_free(struct Dict *dict) {
    for (size_t i = 0; i < dict->length; ++i) {
        MEMORY_FREE(dict->keys[i]);
        MEMORY_FREE(dict->list[i]);
    }
    MEMORY_FREE(dict->list);
    MEMORY_FREE(dict->keys);
    MEMORY_FREE(dict);
}

void dict_add(struct Dict *dict, char *key, char *value) {
    // Resize dict
    if (dict->length + 1 > dict->allocated) {
        dict->list = MEMORY_REALLOCATE(dict->list, dict->allocated * 2 * sizeof(char *));
        dict->keys = MEMORY_REALLOCATE(dict->keys, dict->allocated * 2 * sizeof(char *));
        dict->allocated *= 2;
    }

    long index = dict_key_index(dict, key);
    if (index >= 0) {
        CHARS_COPY_TO(dict->list[index], value);
    } else {
        CHARS_COPY_TO(dict->keys[dict->length], key);
        CHARS_COPY_TO(dict->list[dict->length], value);
        dict->length++;
    }
}

bool dict_value_is(struct Dict *dict, char *key, char *value) {
    char *val = dict_get(dict, key);
    if (val && strcmp(val, value) == 0) return true;
    return false;
}

ssize_t dict_key_index(struct Dict *dict, char *key) {
    for (size_t i = 0; i < dict->length; ++i)
        if (strcmp(dict->keys[i], key) == 0) return i;
    return -1;
}

bool dict_has_key(struct Dict *dict, char *key) {
    for (size_t i = 0; i < dict->length; ++i)
        if (strcmp(dict->keys[i], key) == 0) return true;
    return false;
}

char *dict_get(struct Dict *dict, char *key) {
    for (size_t i = 0; i < dict->length; ++i)
        if (strcmp(dict->keys[i], key) == 0) return dict->list[i];
    return 0;
}

int dict_get_int(struct Dict *dict, char *key) {
    char *val = dict_get(dict, key);
    if (!val) return 0;
    return chars_to_int(val);
}
