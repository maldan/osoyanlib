#include "memory.h"
#include <pthread.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>

size_t GLOBAL_MEMORY_TABLE_SIZE = 0;
struct MemoryBlock **GLOBAL_MEMORY_TABLE;
struct MemoryStatus *GLOBAL_MEMORY_STATUS;

//pthread_t __memoryDebuggerThread228;
//char ** GLOBAL_MEMORY_FUNCTION_STACK = 0;
//char ** GLOBAL_MEMORY_FILE_STACK = 0;
//char ** GLOBAL_MEMORY_LINE_STACK = 0;
//size_t GLOBAL_MEMORY_STACK_SIZE = 0;
//size_t ____start____timer = 0;

/*int guard(int n, char *err) {
    if (n == -1) {
        perror(err);
        exit(1);
    }
    return n;
}*/

/*void ____memory_in(char *fileName, char *function, size_t line) {
    GLOBAL_MEMORY_FILE_STACK[GLOBAL_MEMORY_STACK_SIZE] = fileName;
    GLOBAL_MEMORY_FUNCTION_STACK[GLOBAL_MEMORY_STACK_SIZE] = function;

    char strLine[32];
    memset(strLine, 0, 32);
    sprintf(strLine, "%zu", line);
    GLOBAL_MEMORY_LINE_STACK[GLOBAL_MEMORY_STACK_SIZE] = calloc(1, 32); // line
    memcpy(GLOBAL_MEMORY_LINE_STACK[GLOBAL_MEMORY_STACK_SIZE], strLine, 32);
    GLOBAL_MEMORY_STACK_SIZE++;
}

void ____memory_out() {
    GLOBAL_MEMORY_STACK_SIZE--;
}*/

/*struct MemoryBlock *____memory_find_block_by_pointer(void *pointer) {
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        if (GLOBAL_MEMORY_TABLE[i]->pointer == pointer) {
            return GLOBAL_MEMORY_TABLE[i];
        }
    }
    return 0;
}*/

/*void *____memory_debug_manager(void *vargp) {
    printf("START MEMORY DEBUG SERVER PORT 19501\n");

    int listen_socket_fd = guard(socket(AF_INET, SOCK_STREAM, 0), "could not create TCP listening socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(19501);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    guard(bind(listen_socket_fd, (struct sockaddr *) &addr, sizeof(addr)), "could not bind");
    guard(listen(listen_socket_fd, 100), "could not listen");

    for (;;) {
        //MEMORY_IN;
        // Accept client
        int clientSocketFd = accept(listen_socket_fd, NULL, NULL);

        // Read http query
        char buffer[2048];
        recv(clientSocketFd, buffer, 2048, 0);

        // Error code
        char *response404 = "HTTP/1.1 404 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 3\r\n\r\nsas";

        // If pointer request
        if (strncmp(buffer, "GET /pointer/", 13) == 0) {
            VAR_STRING(finalResponseText);
            void *pointer = (void *)chars_to_long(buffer + 13);

            // Get pointer data
            char *base64Data = "";
            struct MemoryBlock *memoryBlock = ____memory_find_block_by_pointer(pointer);
            if (memoryBlock) base64Data = base64_encode(memoryBlock->pointer, memoryBlock->amount);

            // Final result
            string_add(&finalResponseText, "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: %d\r\n\r\n%s", strlen(base64Data), base64Data);
            send(clientSocketFd, finalResponseText.list, finalResponseText.length, 0);
            DESTROY_STRING(finalResponseText);
        } else
        if (strncmp(buffer, "GET /file/", 10) == 0) {
            VAR_STRING(finalResponseText);

            for (int i = 0; i < 128; ++i) {
                if (buffer[i + 10] == ' ') {
                    buffer[i + 10] = 0;
                    break;
                }
            }

            char *s = "";
            size_t len = 0;
            file_get_contents(buffer + 10, &s, &len);
            // printf("%s ss\n", buffer + 10);

            string_add(&finalResponseText, "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: %d\r\n\r\n%s", len, s);
            send(clientSocketFd, finalResponseText.list, finalResponseText.length, 0);

            DESTROY_STRING(finalResponseText);


        } else
        if (strncmp(buffer, "GET /memory ", 12) == 0) {
            VAR_STRING(finalResponseText);
            VAR_STRING(responseText);

            // Build JSON
            string_add(&responseText, "let info = [");
            for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
                string_add(&responseText, "{");
                string_add(&responseText, "\"id\": %zu, ", i);
                string_add(&responseText, "\"file\": \"%s\", ", GLOBAL_MEMORY_TABLE[i]->fileName);
                string_add(&responseText, "\"line\": %zu, ", GLOBAL_MEMORY_TABLE[i]->line);
                string_add(&responseText, "\"function\": \"%s\", ", GLOBAL_MEMORY_TABLE[i]->function);
                string_add(&responseText, "\"functionPath\": \"%s\", ", GLOBAL_MEMORY_TABLE[i]->functionPath);
                string_add(&responseText, "\"filePath\": \"%s\", ", GLOBAL_MEMORY_TABLE[i]->filePath);
                string_add(&responseText, "\"linePath\": \"%s\", ", GLOBAL_MEMORY_TABLE[i]->linePath);
                string_add(&responseText, "\"type\": %s, ", GLOBAL_MEMORY_TABLE[i]->type);
                string_add(&responseText, "\"time\": %zu, ", GLOBAL_MEMORY_TABLE[i]->time);
                string_add(&responseText, "\"pointer\": %zu, ", GLOBAL_MEMORY_TABLE[i]->pointer);
                string_add(&responseText, "\"amount\": %zu", GLOBAL_MEMORY_TABLE[i]->amount);
                string_add(&responseText, "}, ");
            }
            string_add(&responseText, "];");

            // Final result
            string_add(&finalResponseText, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: %d\r\n\r\n%s", responseText.length, responseText.list);
            send(clientSocketFd, finalResponseText.list, finalResponseText.length, 0);

            DESTROY_STRING(finalResponseText);
            DESTROY_STRING(responseText);
        } else if (strncmp(buffer, "GET / ", 6) == 0) {
            VAR_STRING(finalResponseText);

            char *s = "";
            size_t len;
            size_t sss = file_get_contents("../osoyan/memory/memory.html", &s, &len);

            // Final result
            string_add(&finalResponseText, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: %d\r\n\r\n%s", len, s);
            send(clientSocketFd, finalResponseText.list, finalResponseText.length, 0);
            DESTROY_STRING(finalResponseText);
        } else {
            send(clientSocketFd, response404, strlen(response404), 0);
        }

        close(clientSocketFd);
        //MEMORY_OUT;
    }

    return NULL;
}*/

// Init memory manager
void ____memory_init() {
    GLOBAL_MEMORY_TABLE = calloc(1, 16384 * sizeof(size_t));
    GLOBAL_MEMORY_STATUS = calloc(1, sizeof(struct MemoryStatus));

    /*GLOBAL_MEMORY_FILE_STACK = calloc(1, 16384 * sizeof(size_t));
    GLOBAL_MEMORY_FUNCTION_STACK = calloc(1, 16384 * sizeof(size_t));
    GLOBAL_MEMORY_LINE_STACK = calloc(1, 16384 * sizeof(size_t));*/

    /*struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);
    ____start____timer = start.tv_nsec;*/
    //NEW_STRING_ARRAY(XX);
    //GLOBAL_MEMORY_FUNCTION_STACK = XX;

    // Start debug server
    //pthread_create(&__memoryDebuggerThread228, NULL, ____memory_debug_manager, NULL);
}

// Print info about allocation
void ____memory_print_state() {
    size_t size = 0;
    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        size += GLOBAL_MEMORY_TABLE[i]->size;
        printf("ALLOC[%zu, %p] -> %s() %s:%zu\n", GLOBAL_MEMORY_TABLE[i]->size,
               GLOBAL_MEMORY_TABLE[i]->pointer,
               GLOBAL_MEMORY_TABLE[i]->function,
               GLOBAL_MEMORY_TABLE[i]->fileName,
               GLOBAL_MEMORY_TABLE[i]->line);
    }
    printf("TOTAL ALLOCATION: %zu pointers, [%zu] bytes\n", GLOBAL_MEMORY_STATUS->allocationTotalAmount, GLOBAL_MEMORY_STATUS->allocationTotalSize);
    printf("CURRENT ALLOCATION: %zu pointers, [%zu] bytes\n", GLOBAL_MEMORY_TABLE_SIZE, size);

    // printf("TOTAL[%zu] == %zu ptrs\n", size, GLOBAL_MEMORY_TABLE_SIZE);
}

// Add info about allocation
/*void ____memory_add_block(void *pointer, char *fileName, char *function, size_t line, size_t amount) {

}*/

// Allocate amount and return pointer
void *____memory_allocate(char *fileName, char *function, size_t line, size_t size) {
    void *pointer = calloc(1, size);
    if (!pointer) {
        printf("Can't ALLOC[%zu] -> %s() %s:%zu\n", size, function, file_get_filename(fileName), line);
        exit(1);
    }

    // Don't track memory debugger allocations
    // pthread_t threadId = pthread_self();
    // if (threadId == __memoryDebuggerThread228) return pointer;

    // ____memory_add_block(ptr, file_get_filename(fileName), function, line, amount);
    // struct timespec start;
    // clock_gettime(CLOCK_REALTIME, &start);

    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE] = calloc(1, sizeof(struct MemoryBlock));
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->pointer = pointer;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->fileName = fileName;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->function = function;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->line = line;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->size = size;

    GLOBAL_MEMORY_STATUS->allocationTotalAmount++;
    GLOBAL_MEMORY_STATUS->allocationTotalSize += size;

    // GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->allowedToExitFromFunction = false;
    // GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->type = type;
    // GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->time = start.tv_nsec - ____start____timer;

    // Write path
    /*size_t functionPathSize = 1;
    size_t filePathSize = 1;
    size_t linePathSize = 1;

    for (size_t i = 0; i < GLOBAL_MEMORY_STACK_SIZE; ++i) {
        filePathSize += strlen(GLOBAL_MEMORY_FILE_STACK[i]) + 1;
        functionPathSize += strlen(GLOBAL_MEMORY_FUNCTION_STACK[i]) + 1;
        linePathSize += strlen(GLOBAL_MEMORY_LINE_STACK[i]) + 1;
    }

    char *filePath = calloc(1, filePathSize);
    char *functionPath = calloc(1, functionPathSize);
    char *linePath = calloc(1, linePathSize);

    for (size_t i = 0; i < GLOBAL_MEMORY_STACK_SIZE; ++i) {
        if (i < GLOBAL_MEMORY_STACK_SIZE - 1) {
            sprintf(filePath + strlen(filePath), "%s,", GLOBAL_MEMORY_FILE_STACK[i]);
            sprintf(functionPath + strlen(functionPath), "%s.", GLOBAL_MEMORY_FUNCTION_STACK[i]);
            sprintf(linePath + strlen(linePath), "%s,", GLOBAL_MEMORY_LINE_STACK[i]);
        } else {
            sprintf(filePath + strlen(filePath), "%s", GLOBAL_MEMORY_FILE_STACK[i]);
            sprintf(functionPath + strlen(functionPath), "%s", GLOBAL_MEMORY_FUNCTION_STACK[i]);
            sprintf(linePath + strlen(linePath), "%s", GLOBAL_MEMORY_LINE_STACK[i]);
        }
    }
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->filePath = filePath;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->functionPath = functionPath;
    GLOBAL_MEMORY_TABLE[GLOBAL_MEMORY_TABLE_SIZE]->linePath = linePath;*/

    // Increase table size
    GLOBAL_MEMORY_TABLE_SIZE++;

    return pointer;
}

void *____memory_reallocate(char *fileName, char *function, size_t line, void *pointer, size_t size) {
    // Memory debugger doesn't have memory block record, so just reallocate pointer is enough
    /*pthread_t threadId = pthread_self();
    if (threadId == __memoryDebuggerThread228) {
        void *ptr = realloc(pointer, size);
        return ptr;
    }*/

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

    void *ptr = realloc(pointer, size);
    block->pointer = ptr;
    block->size = size;
    return ptr;
}

// Check pointer in function
/*
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
*/

void ____memory_copy(char *fileName, char *function, size_t line, void *__restrict dst, const void *__restrict src, size_t len, void *__restrict dstStart, size_t dstLen) {
    size_t borderEnd = (size_t) (dstStart + dstLen);
    if ((size_t)(dst + len) > borderEnd) {
        printf("Out of border at %zu bytes in -> %s() %s:%zu\n", ((size_t)(dst + len) - borderEnd), function, file_get_filename(fileName), line);
        exit(1);
    }
    memcpy(dst, src, len);
}

/*void ____memory_debug_manager_await() {
    pthread_join(__memoryDebuggerThread228, NULL);
}*/

// Free pointer
void ____memory_free(char *fileName, char *function, size_t line, char *pointerName, void *pointer) {
    // Memory debugger doesn't have memory block record, so just free pointer is enough
    /*pthread_t threadId = pthread_self();
    if (threadId == __memoryDebuggerThread228) {
        free(pointer);
        return;
    }*/

    for (size_t i = 0; i < GLOBAL_MEMORY_TABLE_SIZE; ++i) {
        if (GLOBAL_MEMORY_TABLE[i]->pointer == pointer) {
            //free(GLOBAL_MEMORY_TABLE[i]->filePath);
            //free(GLOBAL_MEMORY_TABLE[i]->functionPath);
            //free(GLOBAL_MEMORY_TABLE[i]->linePath);

            size_t len = GLOBAL_MEMORY_TABLE_SIZE - (i + 1);
            memmove(GLOBAL_MEMORY_TABLE + i, GLOBAL_MEMORY_TABLE + i + 1, len * sizeof(size_t));
            GLOBAL_MEMORY_TABLE_SIZE--;
            return;
        }
    }
    printf("Trying to free pointer %s[%p] that not found -> %s() %s:%zu\n", pointerName, pointer, function, file_get_filename(fileName), line);
    exit(1);
}