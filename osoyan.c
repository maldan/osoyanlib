#include "osoyan.h"

void default_print(const char *fileName, size_t line, void *data) {
    LOGGER_LOG(fileName, line, "[%p]", data);
    // printf("[%s:%p]", s, v);
}