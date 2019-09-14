#include "osoyan.h"

void default_print(char *s, void *v) {
    printf("[%s:%p]", s, v);
}