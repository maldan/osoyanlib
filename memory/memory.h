#pragma once

#include "../file.h"
#include "../string.h"
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

    //bool allowedToExitFromFunction;
    //const char *type;
    //size_t time;
    //char *functionPath;
    //char *filePath;
    //char *linePath;
};

#define GLOBAL_MEMORY_TABLE_SIZE __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryTableSize
#define GLOBAL_MEMORY_TABLE __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryTable
#define GLOBAL_MEMORY_STATUS __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryStatus

// #define GLOBAL_MEMORY_FUNCTION_STACK __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryDebugFunctionStack
// #define GLOBAL_MEMORY_LINE_STACK __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryDebugLineStack
// #define GLOBAL_MEMORY_FILE_STACK __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryDebugFileStack
// #define GLOBAL_MEMORY_STACK_SIZE __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryDebugStackSize

#ifdef DEBUG
#define MEMORY_INIT ____memory_init()
//#define MEMORY_ALLOCATE_AT(FL, FN, L, A, TYPE) ____memory_allocate(FL, FN, L, #TYPE, A)
#define MEMORY_ALLOCATE(A) ____memory_allocate(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, A)
#define MEMORY_ALLOCATE_STRUCT(A) MEMORY_ALLOCATE(sizeof(struct A), struct A)

#define MEMORY_REALLOCATE(A, S) ____memory_reallocate(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, A, S)
//#define MEMORY_FREE_AT(FL, FN, L, P) ____memory_free(FL, FN, L, #P, P)
#define MEMORY_FREE(P) ____memory_free(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, #P, P)
#define MEMORY_PRINT_STATE ____memory_print_state()
//#define MEMORY_CHECK_FUNCTION ____memory_check_function((char *)__FUNCTION__)
//#define MEMORY_CHECK_TOTAL ____memory_check_total()
//#define MEMORY_ALLOW_EXIT(P) ____memory_allow_exit(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, #P, P)
#define MEMORY_COPY(DST, SRC, SIZE, DST_START, DST_SIZE) ____memory_copy(__BASE_FILE__, (char *)__FUNCTION__, __LINE__, DST, SRC, SIZE, DST_START, DST_SIZE)
//#define MEMORY_AWAIT ____memory_debug_manager_await()
//#define MEMORY_IN ____memory_in(__BASE_FILE__, (char *)__FUNCTION__, __LINE__)
//#define MEMORY_OUT ____memory_out()
#else
#define MEMORY_INIT NULL
#define MEMORY_ALLOCATE(A) calloc(1, A)
//#define MEMORY_ALLOCATE_AT(F, A) calloc(1, A)
#define MEMORY_ALLOCATE_STRUCT(A) calloc(1, sizeof(A))
//#define MEMORY_FREE_AT(FL, FN, L, P) free(P)
#define MEMORY_FREE(P) free(P)
#define MEMORY_PRINT_STATE NULL
//#define MEMORY_CHECK_FUNCTION NULL
//#define MEMORY_CHECK_TOTAL NULL
//#define MEMORY_ALLOW_EXIT(P) NULL
//#define MEMORY_AWAIT NULL
#define MEMORY_COPY(DST, SRC, SIZE, DST_START, DST_SIZE) memcpy(DST, SRC, SIZE)
#endif

// Init memory manager
void ____memory_init();

// Memory function in
//void ____memory_in(char *fileName, char *function, size_t line);

// Memory function out
//void ____memory_out();

// Don't close program, keep debugger working
//void ____memory_debug_manager_await();

// Print info about allocation
void ____memory_print_state();

// Allocate amount and return pointer
void *____memory_allocate(char *fileName, char *function, size_t line, size_t amount);

// Reallocate amount and return pointer
void *____memory_reallocate(char *fileName, char *function, size_t line, void *pointer, size_t amount);

// Check pointer in function
//void ____memory_check_function(char *function);

// Check all pointers
//void ____memory_check_total();

// Allow point to exit from function
//void ____memory_allow_exit(char *fileName, char *function, size_t line, char *pointerName, void *pointer);

// Allow point to exit from function
void ____memory_copy(char *fileName, char *function, size_t line, void *__restrict dst, const void *__restrict src, size_t len, void *__restrict dstStart, size_t dstLen);

// Free pointer
void ____memory_free(char *fileName, char *function, size_t line, char *pointerName, void *pointer);