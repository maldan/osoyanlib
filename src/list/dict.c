#include "../../include/list/dict.h"

void ____dict_init(struct Dict *dict, const char *type) {
    dict->list = MEMORY_ALLOCATE(sizeof(void **) * 4);
    dict->keys = MEMORY_ALLOCATE(sizeof(char **) * 4);
    dict->length = 0;
    dict->allocated = 4;
    dict->type = type;
}

void ____dict_free(struct Dict *dict) {
    MEMORY_FREE(dict->list);
    for (size_t i = 0; i < dict->length; ++i)
        MEMORY_FREE(dict->keys[i]);
    MEMORY_FREE(dict->keys);
    MEMORY_FREE(dict);
}

void dict_add(struct Dict *dict, char *key, void *value) {
    // Resize dict
    if (dict->length + 1 > dict->allocated) {
        dict->list = MEMORY_REALLOCATE(dict->list, dict->allocated * 2 * sizeof(void *));
        dict->keys = MEMORY_REALLOCATE(dict->keys, dict->allocated * 2 * sizeof(char *));
        dict->allocated *= 2;
    }

    long index = dict_key_index(dict, key);
    if (index >= 0) {
        dict->list[index] = value;
    } else {
        CLONE_CHARS(dict->keys[dict->length], key, tmp);
        dict->list[dict->length] = value;
        dict->length++;
    }
}

long dict_key_index(struct Dict *dict, char *key) {
    for (size_t i = 0; i < dict->length; ++i)
        if (strcmp(dict->keys[i], key) == 0) return i;
    return -1;
}

/*int dict_get(struct Dict *dict, char *key, void **out) {
    for (uint32_t i = 0; i < dict->size; ++i) {
        if (strcmp(dict->keys[i], key) == 0) {
            *out = dict->values[i];
            return 1;
        }
    }
    return 0;
}

int dict_find_index(struct Dict *dict, char *key, size_t *index) {
    for (size_t i = 0; i < dict->size; ++i) {
        if (strcmp(dict->keys[i], key) == 0) {
            *index = i;
            return 1;
        }
    }
    return 0;
}

void dict_print(struct Dict *dict) {
    printf("dict <%s> {\n", dict->type);
    if (strcmp(dict->type, "string") == 0) {
        for (uint64_t i = 0; i < dict->size; ++i)
            printf("    %s => %s\n", dict->keys[i], (char *)dict->values[i]);
    }
    else {
        for (uint64_t i = 0; i < dict->size; ++i)
            printf("    %s => %zu\n", dict->keys[i], (size_t)dict->values[i]);
    }
    printf("}\n");
}*/