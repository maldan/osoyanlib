#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <sys/types.h>

struct MemoryStatus {
    size_t allocationTotalAmount;
    size_t allocationTotalSize;
};

struct MemoryBlock {
    void *pointer;
    char *fileName;
    size_t line;
    size_t size;
};

#define GLOBAL_MEMORY_TABLE_SIZE __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryTableSize
#define GLOBAL_MEMORY_TABLE __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryTable
#define GLOBAL_MEMORY_STATUS __UNIQSALT2281488OSOYANFOREVER_AND_SOSO__globalMemoryStatus

#ifdef DEBUG
#define MEMORY_INIT ____memory_init();
// #define MEMORY_ALLOCATE_AT(FL, L, A, TYPE) ____memory_allocate(FL, L, #TYPE, A)
#define MEMORY_ALLOCATE(A) ____memory_allocate(__BASE_FILE__, __LINE__, A)
#define MEMORY_ALLOCATE_STRUCT(A) MEMORY_ALLOCATE(sizeof(struct A))
#define MEMORY_REALLOCATE(A, S) ____memory_reallocate(__BASE_FILE__, __LINE__, A, S)
#define MEMORY_FREE(P) ____memory_free(__BASE_FILE__, __LINE__, #P, P);
#define MEMORY_PRINT_STATE ____memory_print_state(false);
#define MEMORY_WPRINT_STATE { char *__xxx228 = ____memory_print_state(true);\
NEW_WSTRING(X) INIT_WSTRING(X, __xxx228) wprintf(L"\n%ls", X->list); };
#define MEMORY_COPY(DST, SRC, SIZE, DST_START, DST_SIZE) ____memory_copy(__BASE_FILE__, __LINE__, DST, SRC, SIZE, DST_START, DST_SIZE)
#define MEMORY_IS_FREE ____memory_is_free()
#else
#define MEMORY_INIT NULL;
//#define MEMORY_ALLOCATE_AT(FL, FN, L, A) calloc(1, A)
#define MEMORY_ALLOCATE(A) calloc(1, A)
#define MEMORY_ALLOCATE_STRUCT(A) calloc(1, sizeof(struct A))
#define MEMORY_REALLOCATE(A, S) realloc(A, S)
#define MEMORY_FREE(P) free(P);
#define MEMORY_PRINT_STATE NULL;
#define MEMORY_WPRINT_STATE NULL;
#define MEMORY_COPY(DST, SRC, SIZE, DST_START, DST_SIZE) memcpy(DST, SRC, SIZE)
#define MEMORY_IS_FREE true
#endif

/**
 * Init memory manager. You need put macros !MEMORY_INIT! at the start of ~main~ function.
 */
void ____memory_init();

/**
 * Prints current state of memory allocation. It prints all allocated pointers and places where allocation was made.
 * It also prints total allocations of amount of pointers and total size in bytes. And also prints current allocation.
 * If your current allocation is 0 pointer and [0] bytes it's cool, that means you free all memory you allocated and you
 * doesn't have memory leak. Note, if you free pointer then information about allocation about the pointer removes from global table.
 */
char * ____memory_print_state(bool writeInBuffer);

char * ____memory_get_state();

/**
 * Function similar to ~calloc~. It's just add information of allocation to the global table.
 * @param fileName - File where allocation was made, usually results of macros !__FILE__!
 * @param line - Line where allocation was made, usually results of macros !__LINE__!
 * @param amount - Amount of allocated memory
 * @return Returns pointer to allocated memory block
 */
void *____memory_allocate(char *fileName, size_t line, size_t amount);

/**
 * Function similar to ~realloc~. But you can't reallocate pointer if it wasn't allocated by ~~____memory_allocate~~.
 * @param fileName - File where allocation was made, usually results of macros !__FILE__!
 * @param line - Line where allocation was made, usually results of macros !__LINE__!
 * @param pointer - Pointer to reallocate memory
 * @param amount - New amount of memory
 * @return Returns pointer to reallocated memory block
 */
void *____memory_reallocate(char *fileName, size_t line, void *pointer, size_t amount);

/**
 * Function similar to ~memcpy~. But it also take additional arguments for more memory safety.
 * You have to pass start of pointer $dstStart$, not offset like ptr + 5, but ptr itself. And you also need to pass total size of memory block of ptr.
 * For example you allocated 10 bytes by ptr = malloc(10). Now you have to pass ptr as $dstStart$ and 10 as $dstLen$.
 * So function can detect if you put something out of $dst$ range. But it doesn't check $src$ size.
 * @param fileName - File where allocation was made, usually results of macros !__FILE__!
 * @param line - Line where allocation was made, usually results of macros !__LINE__!
 * @param dst - Destination of memory block
 * @param src - Source of memory block
 * @param len - Length of copied data
 * @param dstStart - Start of destination pointer
 * @param dstLen - Total size of memory block of destination pointer
 */
void ____memory_copy(char *fileName, size_t line, void *__restrict dst, const void *__restrict src, size_t len, void *__restrict dstStart, size_t dstLen);

/**
 * Function similar to ~free~ but it also removes information about allocation from global table.
 * @param fileName - File where allocation was made, usually results of macros !__FILE__!
 * @param line - Line where allocation was made, usually results of macros !__LINE__!
 * @param pointerName - Pointer or var name, need only for print
 * @param pointer - Pointer you allocated
 */
void ____memory_free(char *fileName, size_t line, char *pointerName, void *pointer);

/**
 * Function used to check if memory table is empty or not.
 * @return - Return !true! if allocation table is empty otherwise return !false!
 */
bool ____memory_is_free();
