#pragma once

#include "../io/file.h"
#include "../string/string.h"
#include <stdbool.h>

struct MemoryStatus {
    size_t allocationTotalAmount;
    size_t allocationTotalSize;
};

struct MemoryBlock {
    void *pointer;
    char *fileName;
    char *function;
    size_t line;
    size_t size;
};

#define GLOBAL_MEMORY_TABLE_SIZE __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryTableSize
#define GLOBAL_MEMORY_TABLE __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryTable
#define GLOBAL_MEMORY_STATUS __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryStatus


#ifdef DEBUG
#define MEMORY_INIT ____memory_init()
#define MEMORY_ALLOCATE_AT(FL, FN, L, A, TYPE) ____memory_allocate(FL, FN, L, #TYPE, A)
#define MEMORY_ALLOCATE(A) ____memory_allocate(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, A)
#define MEMORY_ALLOCATE_STRUCT(A) MEMORY_ALLOCATE(sizeof(struct A))
#define MEMORY_REALLOCATE(A, S) ____memory_reallocate(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, A, S)
#define MEMORY_FREE(P) ____memory_free(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, #P, P)
#define MEMORY_PRINT_STATE ____memory_print_state()
#define MEMORY_COPY(DST, SRC, SIZE, DST_START, DST_SIZE) ____memory_copy(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, DST, SRC, SIZE, DST_START, DST_SIZE)
#else
#define MEMORY_INIT NULL
#define MEMORY_ALLOCATE_AT(FL, FN, L, A) calloc(1, A)
#define MEMORY_ALLOCATE(A) calloc(1, A)
#define MEMORY_ALLOCATE_STRUCT(A) calloc(1, sizeof(struct A))
#define MEMORY_REALLOCATE(A, S) realloc(A, S)
#define MEMORY_FREE(P) free(P)
#define MEMORY_PRINT_STATE NULL
#define MEMORY_COPY(DST, SRC, SIZE, DST_START, DST_SIZE) memcpy(DST, SRC, SIZE)
#endif

// Init memory manager
void ____memory_init();

// Print info about allocation
void ____memory_print_state();

// Allocate amount and return pointer
void *____memory_allocate(char *fileName, char *function, size_t line, size_t amount);

// Reallocate amount and return pointer
void *____memory_reallocate(char *fileName, char *function, size_t line, void *pointer, size_t amount);

// Allow point to exit from function
void ____memory_copy(char *fileName, char *function, size_t line, void *__restrict dst, const void *__restrict src, size_t len, void *__restrict dstStart, size_t dstLen);

// Free pointer
void ____memory_free(char *fileName, char *function, size_t line, char *pointerName, void *pointer);