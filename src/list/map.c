#include "../../include/list/map.h"

void ____map_init(struct Map *map, const char *type) {
    map->list = MEMORY_ALLOCATE(sizeof(void **) * 4);
    map->keys = MEMORY_ALLOCATE(sizeof(char **) * 4);
    map->length = 0;
    map->allocated = 4;
    map->type = type;
}

void ____map_free(struct Map *map) {
    for (size_t i = 0; i < map->length; ++i) {
        MEMORY_FREE(map->keys[i]);
        MEMORY_FREE(map->list[i]);
    }
    MEMORY_FREE(map->list);
    MEMORY_FREE(map->keys);
    MEMORY_FREE(map);
}

void map_add(struct Map *map, char *key, void *value) {
    // Resize dict
    if (map->length + 1 > map->allocated) {
        map->list = MEMORY_REALLOCATE(map->list, map->allocated * 2 * sizeof(void *));
        map->keys = MEMORY_REALLOCATE(map->keys, map->allocated * 2 * sizeof(char *));
        map->allocated *= 2;
    }

    long index = map_key_index(map, key);
    if (index >= 0) {
        map->list[index] = value;
    } else {
        CHARS_COPY_TO(map->keys[map->length], key);
        map->list[map->length] = value;
        map->length++;
    }
}

void *map_get(struct Map *map, char *key) {
    for (size_t i = 0; i < map->length; ++i)
        if (strcmp(map->keys[i], key) == 0) return map->list[i];
    return 0;
}

ssize_t map_key_index(struct Map *map, char *key) {
    for (size_t i = 0; i < map->length; ++i)
        if (strcmp(map->keys[i], key) == 0) return i;
    return -1;
}
