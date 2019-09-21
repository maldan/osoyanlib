#include "../../include/memory/memory.h"
#include <sys/types.h>

size_t GLOBAL_MEMORY_TABLE_SIZE = 0;
struct MemoryBlock **GLOBAL_MEMORY_TABLE;
struct MemoryStatus *GLOBAL_MEMORY_STATUS;

// Init memory manager
void ____memory_init() {
    GLOBAL_MEMORY_TABLE = calloc(1, 16384 * sizeof(size_t));
    GLOBAL_MEMORY_STATUS = calloc(1, sizeof(struct MemoryStatus));
}

// Print info about allocation
void ____memory_print_state() {
    size_t size = 0;
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        size += GLOBAL_MEMORY_TABLE[i]->size;
        printf("ALLOC[%zu, %p] -> %s:%zu\n", GLOBAL_MEMORY_TABLE[i]->size,
               GLOBAL_MEMORY_TABLE[i]->pointer,
               GLOBAL_MEMORY_TABLE[i]->fileName,
               GLOBAL_MEMORY_TABLE[i]->line);
    }
    printf("TOTAL ALLOCATION: %zu pointers, [%zu] bytes\n", GLOBAL_MEMORY_STATUS->allocationTotalAmount, GLOBAL_MEMORY_STATUS->allocationTotalSize);
    printf("CURRENT ALLOCATION: %zu pointers, [%zu] bytes\n", GLOBAL_MEMORY_TABLE_SIZE, size);
}

// Allocate amount and return pointer
void *____memory_allocate(char *fileName, size_t line, size_t size) {
    void *pointer = calloc(1, size);
    if (!pointer) {
        printf("Can't ALLOC[%zu] -> %s:%zu\n", size, fileName, line);
        exit(1);
    }

    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE] = calloc(1, sizeof(struct MemoryBlock));
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->pointer = pointer;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->fileName = fileName;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->line = line;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->size = size;

    GLOBAL_MEMORY_STATUS->allocationTotalAmount++;
    GLOBAL_MEMORY_STATUS->allocationTotalSize += size;

    // Increase table size
    GLOBAL_MEMORY_TABLE_SIZE++;

    return pointer;
}

void *____memory_reallocate(char *fileName, size_t line, void *pointer, size_t size) {
    struct MemoryBlock *block = 0;
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        if (GLOBAL_MEMORY_TABLE[i]->pointer == pointer) {
            block = GLOBAL_MEMORY_TABLE[i];
            break;
        }
    }

    if (!block) {
        fprintf( stderr,"Can't found %p pointer to reallocation -> %s:%zu\n", pointer, fileName, line);
        exit(1);
    }

    // Change reallocated size
    GLOBAL_MEMORY_STATUS->allocationTotalSize -= block->size;
    GLOBAL_MEMORY_STATUS->allocationTotalSize += size;

    void *ptr = realloc(pointer, size);
    block->pointer = ptr;
    block->size = size;
    return ptr;
}

void ____memory_copy(char *fileName, size_t line, void *__restrict dst, const void *__restrict src, size_t len, void *__restrict dstStart, size_t dstLen) {
    size_t borderEnd = (size_t) (dstStart + dstLen);
    if ((size_t)(dst + len) > borderEnd) {
        printf("Out of border at %zu bytes in -> %s:%zu\n", ((size_t)(dst + len) - borderEnd), fileName, line);
        exit(1);
    }
    memcpy(dst, src, len);
}

// Free pointer
void ____memory_free(char *fileName, size_t line, char *pointerName, void *pointer) {
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        if (GLOBAL_MEMORY_TABLE[i]->pointer == pointer) {
            size_t len = GLOBAL_MEMORY_TABLE_SIZE - (i + 1);
            memmove(GLOBAL_MEMORY_TABLE + i, GLOBAL_MEMORY_TABLE + i + 1, len * sizeof(size_t));
            GLOBAL_MEMORY_TABLE_SIZE--;
            return;
        }
    }
    printf("Trying to free pointer %s[%p] that not found -> %s:%zu\n", pointerName, pointer, fileName, line);
    exit(1);
}

bool ____memory_is_free() {
    return GLOBAL_MEMORY_TABLE_SIZE <= 0;
}
