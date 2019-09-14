#pragma once

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "../osoyan.h"

#define NEW_BLOB(X) struct Blob *X = MEMORY_ALLOCATE_STRUCT(Blob); ____blob_init(X)
#define DESTROY_BLOB(X) ____blob_free(X)

struct Blob {
    uint8_t *list;
    size_t length;
    size_t allocated;
    size_t position;
};

void ____blob_init(struct Blob *blob);
void ____blob_free(struct Blob *blob);

void blob_put(struct Blob *blob, uint64_t value, size_t size, size_t position);
void blob_put_auto(struct Blob *blob, uint64_t value, size_t position);
void blob_put8(struct Blob *blob, int8_t value, size_t position);
void blob_put16(struct Blob *blob, int16_t value, size_t position);
void blob_put32(struct Blob *blob, int32_t value, size_t position);
void blob_put64(struct Blob *blob, int64_t value, size_t position);
void blob_put_chars(struct Blob *blob, char *chars, size_t position);
void blob_concat(struct Blob *dst, struct Blob *src);

void print_blob(char *fileName, size_t line, struct Blob *blob);
// void blob_print(struct Blob *dst, size_t size);