#include "../../include/list/blob.h"

void ____blob_init(struct Blob *blob) {
    INIT_DEFAULT_LIST_SIZE(blob, uint8_t);
}

void ____blob_free(struct Blob *blob) {
    MEMORY_FREE(blob->list);
    MEMORY_FREE(blob);
}

void blob_allocate(struct Blob *blob, size_t amount) {
    blob->list = MEMORY_REALLOCATE(blob->list,  blob->allocated + amount * sizeof(uint8_t));
    blob->allocated = blob->allocated + amount * sizeof(uint8_t);
}

void blob_put(struct Blob *blob, uint64_t value, size_t size, ssize_t position) {
    RESIZE_ARRAY_IF_NEED(blob, size, uint8_t)

    // Copy value
    if (position == -1) {
        MEMORY_COPY(blob->list + blob->position, &value, size, blob->list, blob->allocated);
        blob->position += size;
        blob->length += size;
    } else {
        assert(position <= (long)(blob->allocated - size));
        MEMORY_COPY(blob->list + position, &value, size, blob->list, blob->allocated);
    }
}

void blob_put8(struct Blob *blob, int8_t value, ssize_t position) {
    blob_put(blob, value, 1, position);
}
void blob_put16(struct Blob *blob, int16_t value, ssize_t position) {
    blob_put(blob, value, 2, position);
}
void blob_put32(struct Blob *blob, int32_t value, ssize_t position) {
    blob_put(blob, value, 4, position);
}
void blob_put64(struct Blob *blob, int64_t value, ssize_t position) {
    blob_put(blob, value, 8, position);
}

/*void blob_put_auto(struct Blob *blob, uint64_t value, size_t position) {
    if (value <= 255) blob_put8(blob, value, position);
    else if (value <= 65536) blob_put16(blob, value, position);
    else if (value <= 4294967295) blob_put32(blob, value, position);
    else blob_put64(blob, value, position);
}*/

void blob_put_chars(struct Blob *blob, char *chars, ssize_t position) {
    // @TODO position is ignore
    for (size_t i = 0; i < strlen(chars); ++i) {
        blob_put8(blob, chars[i], position);
    }
}

void blob_put_bytes(struct Blob *blob, uint8_t *bytes, ssize_t position, size_t size) {
    // @TODO position is ignore
    for (size_t i = 0; i < size; ++i) {
        blob_put8(blob, bytes[i], position);
    }
}

void blob_concat(struct Blob *dst, struct Blob *src) {
    // Resize blob
    if (dst->position + src->length > dst->allocated) {
        dst->list = MEMORY_REALLOCATE(dst->list, (dst->allocated + src->length) * sizeof(char));
        dst->allocated += src->length * sizeof(char);
    }
    MEMORY_COPY(dst->list + dst->position, src->list, src->length, dst->list, dst->allocated);
    dst->length += src->length;
    dst->position += src->length;
}
