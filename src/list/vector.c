#include "../../include/list/vector.h"

void ____vector_init(struct Vector *vector, const char *type) {
    INIT_DEFAULT_LIST_SIZE(vector, void *);
    vector->type = type;
}

void ____vector_free(struct Vector *vector) {
    MEMORY_FREE(vector->list);
    MEMORY_FREE(vector);
}

void vector_push(struct Vector *vector, void *value) {
    RESIZE_ARRAY_IF_NEED(vector, 1, void *);
    vector->list[vector->length++] = value;
}

void *vector_get(struct Vector *vector, ssize_t position) {
    if (position < 0) return 0;
    if ((size_t)position >= vector->length) return 0;
    return vector->list[position];
}

void vector_remove_at(struct Vector *vector, ssize_t at, size_t amount) {
    ARRAY_REMOVE_AT(vector, size_t);
}

void vector_clear(struct Vector *vector) {
    memset(vector->list, 0, vector->length * sizeof(void *));
    vector->length = 0;
}

/*int vector_get(struct Vector *vector, uint64_t position, void **out) {
    if (position >= vector->size) return 0;
    *out = vector->list[position];
    return 1;
}

void vector_push(struct Vector *vector, void *value) {
    // Resize array
    if (vector->size + 1 > vector->allocated) {
        vector->list = realloc(vector->list, vector->allocated * 2 * sizeof(size_t));
        vector->allocated *= 2;
    }
    vector->list[vector->size] = value;
    vector->size++;
}*/

/*void vector_remove_at(struct Vector *vector, size_t at, size_t amount) {
    size_t len = vector->size - (at + 1);
    //for (size_t i = 0; i < len; ++i)
    //    vector->list[at + i] = vector->list[at + amount + i];
    memmove(vector->list + at, vector->list + at + amount, len * sizeof(size_t));
    vector->size -= amount;
}

void vector_insert_at(struct Vector *vector, size_t at, void *value) {
    // Resize array if need
    if (vector->size + 1 > vector->allocated) {
        vector->list = realloc(vector->list, vector->allocated * 2 * sizeof(size_t));
        vector->allocated *= 2;
    }

    size_t len = vector->size - at;
    vector->size++;
    for (size_t i = 0; i <= len; ++i) {
        vector->list[vector->size - i] = vector->list[vector->size - i - 1];
    }

    vector->list[at] = value;
}*/

/*void vector_print(struct Vector *vector, char *format) {
    printf("vector {\n");

    if (strcasecmp(format, "string") == 0) {
        for (size_t i = 0; i < vector->size; ++i)
            printf("    %lu => \"%s\"\n", i, (char *)vector->list[i]);
    } else {
        for (size_t i = 0; i < vector->size; ++i)
            printf("    %lu => %ld\n", i, (long)vector->list[i]);
    }

    printf("}\n");
}
*/
