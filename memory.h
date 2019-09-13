#pragma once

#include "file.h"
#include <stdbool.h>

struct MemoryBlock {
    bool allowedToExitFromFunction;
    void *pointer;
    char *fileName;
    char *function;
    size_t line;
    size_t amount;
};

#define GLOBAL_MEMORY_TABLE_SIZE __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryTableSize
#define GLOBAL_MEMORY_TABLE __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryTable

#ifdef DEBUG
#define MEMORY_INIT ____memory_init()
#define MEMORY_ALLOCATE_AT(FL, FN, L, A) ____memory_allocate(FL, FN, L, A)
#define MEMORY_ALLOCATE(A) ____memory_allocate(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, A)
#define MEMORY_REALLOCATE(A, S) ____memory_reallocate(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, A, S)
#define MEMORY_ALLOCATE_STRUCT(A) MEMORY_ALLOCATE(sizeof(struct A))
#define MEMORY_FREE_AT(FL, FN, L, P) ____memory_free(FL, FN, L, #P, P)
#define MEMORY_FREE(P) ____memory_free(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, #P, P)
#define MEMORY_PRINT_STATE ____memory_print_state()
#define MEMORY_CHECK_FUNCTION ____memory_check_function((char *)__FUNCTION__)
#define MEMORY_CHECK_TOTAL ____memory_check_total()
#define MEMORY_ALLOW_EXIT(P) ____memory_allow_exit(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, #P, P)
#define MEMORY_COPY(DST, SRC, SIZE, DST_START, DST_SIZE) ____memory_copy(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, DST, SRC, SIZE, DST_START, DST_SIZE)
#else
#define MEMORY_INIT NULL
#define MEMORY_ALLOCATE(A) calloc(1, A)
#define MEMORY_ALLOCATE_AT(F, A) calloc(1, A)
#define MEMORY_ALLOCATE_STRUCT(A) calloc(1, sizeof(A))
#define MEMORY_FREE_AT(FL, FN, L, P) free(P)
#define MEMORY_FREE(P) free(P)
#define MEMORY_PRINT_STATE NULL
#define MEMORY_CHECK_FUNCTION NULL
#define MEMORY_CHECK_TOTAL NULL
#define MEMORY_ALLOW_EXIT(P) NULL
#endif

// Init memory manager
void ____memory_init();

void ____memory_sex();

// Print info about allocation
void ____memory_print_state();

// Add info about allocation
void ____memory_add_block(void *pointer, char *fileName, char *function, size_t line, size_t amount);

// Allocate amount and return pointer
void *____memory_allocate(char *fileName, char *function, size_t line, size_t amount);

// Reallocate amount and return pointer
void *____memory_reallocate(char *fileName, char *function, size_t line, void *pointer, size_t amount);

// Check pointer in function
void ____memory_check_function(char *function);

// Check all pointers
void ____memory_check_total();

// Allow point to exit from function
void ____memory_allow_exit(char *fileName, char *function, size_t line, char *pointerName, void *pointer);

// Allow point to exit from function
void ____memory_copy(char *fileName, char *function, size_t line, void *__restrict dst, const void *__restrict src, size_t len, void *__restrict dstStart, size_t dstLen);

// Free pointer
void ____memory_free(char *fileName, char *function, size_t line, char *pointerName, void *pointer);