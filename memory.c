#include "memory.h"

size_t GLOBAL_MEMORY_TABLE_SIZE = 0;
struct MemoryBlock **GLOBAL_MEMORY_TABLE;

// Init memory manager
void ____memory_init() {
    GLOBAL_MEMORY_TABLE = calloc(1, 16384 * sizeof(size_t));
}

// Print info about allocation
void ____memory_print_state() {
    size_t amount = 0;
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        amount += GLOBAL_MEMORY_TABLE[i]->amount;
        printf("ALLOC[%zu, %p] -> %s() %s:%zu\n", GLOBAL_MEMORY_TABLE[i]->amount,
               GLOBAL_MEMORY_TABLE[i]->pointer,
               GLOBAL_MEMORY_TABLE[i]->function,
               GLOBAL_MEMORY_TABLE[i]->fileName,
               GLOBAL_MEMORY_TABLE[i]->line);
    }
    printf("TOTAL[%zu] == %zu ptrs\n", amount, GLOBAL_MEMORY_TABLE_SIZE);
}

// Add info about allocation
void ____memory_add_block(void *pointer, char *fileName, char *function, size_t line, size_t amount) {
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE] = calloc(1, sizeof(struct MemoryBlock));
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->allowedToExitFromFunction = false;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->pointer = pointer;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->fileName = fileName;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->function = function;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->line = line;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->amount = amount;
    GLOBAL_MEMORY_TABLE_SIZE++;
}

// Allocate amount and return pointer
void *____memory_allocate(char *fileName, char *function, size_t line, size_t amount) {
    void *ptr = calloc(1, amount);
    if (!ptr) {
        printf("Can't ALLOC[%zu] -> %s() %s:%zu\n", amount, function, file_get_filename(fileName), line);
        exit(1);
    }
    ____memory_add_block(ptr, file_get_filename(fileName), function, line, amount);
    return ptr;
}

void *____memory_reallocate(char *fileName, char *function, size_t line, void *pointer, size_t amount) {
    struct MemoryBlock *block = 0;
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        if (GLOBAL_MEMORY_TABLE[i]->pointer == pointer) {
            block = GLOBAL_MEMORY_TABLE[i];
            break;
        }
    }

    if (!block) {
        fprintf( stderr,"Can't found %p pointer to reallocation -> %s() %s:%zu\n", pointer, function, fileName, line);
        exit(1);
    }

    void *ptr = realloc(pointer, amount);
    block->pointer = ptr;
    block->amount = amount;
    return ptr;
}

// Check pointer in function
void ____memory_check_function(char *function) {
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        if (strcmp(function, GLOBAL_MEMORY_TABLE[i]->function) == 0) {
            if (GLOBAL_MEMORY_TABLE[i]->allowedToExitFromFunction) {
                GLOBAL_MEMORY_TABLE[i]->allowedToExitFromFunction = false;
                continue;
            }

            fprintf( stderr,"You must free %p pointer -> %s() %s:%zu\n",
                    GLOBAL_MEMORY_TABLE[i]->pointer,
                    GLOBAL_MEMORY_TABLE[i]->function,
                    GLOBAL_MEMORY_TABLE[i]->fileName,
                    GLOBAL_MEMORY_TABLE[i]->line);
            exit(1);
        }
    }
}

void ____memory_check_total() {
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        if (GLOBAL_MEMORY_TABLE[i]->allowedToExitFromFunction) {
            GLOBAL_MEMORY_TABLE[i]->allowedToExitFromFunction = false;
            continue;
        }

        fprintf( stderr,"You must free %p pointer -> %s() %s:%zu\n",
                 GLOBAL_MEMORY_TABLE[i]->pointer,
                 GLOBAL_MEMORY_TABLE[i]->function,
                 GLOBAL_MEMORY_TABLE[i]->fileName,
                 GLOBAL_MEMORY_TABLE[i]->line);
        exit(1);
    }
}

// Allow point to exit from function
void ____memory_allow_exit(char *fileName, char *function, size_t line, char *pointerName, void *pointer) {
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        if (GLOBAL_MEMORY_TABLE[i]->pointer == pointer) {
            GLOBAL_MEMORY_TABLE[i]->allowedToExitFromFunction = true;
            return;
        }
    }

    printf("Pointer %s[%p] not found -> %s() %s:%zu\n", pointerName, pointer, function, file_get_filename(fileName), line);
    exit(1);
}

void ____memory_copy(char *fileName, char *function, size_t line, void *__restrict dst, const void *__restrict src, size_t len, void *__restrict dstStart, size_t dstLen) {
    size_t borderEnd = (size_t) (dstStart + dstLen);
    if ((size_t)(dst + len) > borderEnd) {
        printf("Out of border at %zu bytes in -> %s() %s:%zu\n", ((size_t)(dst + len) - borderEnd), function, file_get_filename(fileName), line);
        exit(1);
    }
    memcpy(dst, src, len);
}

// Free pointer
void ____memory_free(char *fileName, char *function, size_t line, char *pointerName, void *pointer) {
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        if (GLOBAL_MEMORY_TABLE[i]->pointer == pointer) {
            size_t len = GLOBAL_MEMORY_TABLE_SIZE - (i + 1);
            memmove(GLOBAL_MEMORY_TABLE + i, GLOBAL_MEMORY_TABLE + i + 1, len * sizeof(size_t));
            GLOBAL_MEMORY_TABLE_SIZE--;
            return;
        }
    }
    printf("Trying to free pointer %s[%p] that not found -> %s() %s:%zu\n", pointerName, pointer, function, file_get_filename(fileName), line);
    exit(1);
}