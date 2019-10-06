#pragma once

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "list.h"
#include "../memory/memory.h"
#include "../string/string.h"

#define NEW_BLOB(X) struct Blob *X = MEMORY_ALLOCATE_STRUCT(Blob); ____blob_init(X);
#define INIT_BLOB(X) (X) = MEMORY_ALLOCATE_STRUCT(Blob); ____blob_init(X);
#define EQU_BLOB(X) struct Blob *X
#define DESTROY_BLOB(X) ____blob_free(X);

struct Blob {
    uint8_t *list;
    size_t length;
    size_t allocated;
    size_t position;
};

/**
 * Init byte array
 * @param blob - Current blob
 */
void ____blob_init(struct Blob *blob);
/**
 * Destroy byte array
 * @param blob - Current blob
 */
void ____blob_free(struct Blob *blob);

void blob_allocate(struct Blob *blob, size_t amount);

/**
 * Add $value$ to specified $position$ in byte array $blob$ with specified $size$ of element.
 * For example if size is 1 then function add 1 byte to $blob$. If size is 4 it add %int%.
 * Function add $value$ to the end of byte array if $position$ is -1.
 * @param blob - Current byte array
 * @param value - Number to add, can be %char% or %int% and etc because result depends on $size$
 * @param size - Size of value in bytes
 * @param position - Position in byte array
 */
void blob_put(struct Blob *blob, uint64_t value, size_t size, ssize_t position);

/**
 * Function works same as ~blob_put~ but the $size$ is 1 byte.
 * @param blob - Current byte array
 * @param value - Number to add, max size is 1 byte
 * @param position - Position in byte array
 */
void blob_put8(struct Blob *blob, int8_t value, ssize_t position);

/**
 * Function works same as ~blob_put~ but the $size$ is 2 byte.
 * @param blob - Current byte array
 * @param value - Number to add, max size is 2 byte
 * @param position - Position in byte array
 */
void blob_put16(struct Blob *blob, int16_t value, ssize_t position);

/**
 * Function works same as ~blob_put~ but the $size$ is 4 byte.
 * @param blob - Current byte array
 * @param value - Number to add, max size is 4 byte
 * @param position - Position in byte array
 */
void blob_put32(struct Blob *blob, int32_t value, ssize_t position);

/**
 * Function works same as ~blob_put~ but the $size$ is 8 byte.
 * @param blob - Current byte array
 * @param value - Number to add, max size is 8 byte
 * @param position - Position in byte array
 */
void blob_put64(struct Blob *blob, int64_t value, ssize_t position);

/**
 * Add $chars$ to $blob$ array at specified $position$. Add $chars$ to the end of array if $position$ is -1
 * @param blob - Current byte array
 * @param chars - Chars you want to add
 * @param position - Position in byte array
 */
void blob_put_chars(struct Blob *blob, char *chars, ssize_t position);

void blob_put_bytes(struct Blob *blob, uint8_t *bytes, size_t size, ssize_t position);

void blob_concat(struct Blob *blob, struct Blob *src);
