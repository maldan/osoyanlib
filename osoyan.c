#include "osoyan.h"

char * print_default(const char *fileName, size_t line, void *data, bool writeToBuffer) {
    LOGGER_LOG(fileName, line, "[%p]", data);
    // printf("[%s:%p]", s, v);
    return 0;
}