#include "osoyan.h"

void chars_print(char *s, void *v) {
    printf("%s", (char *)v);
}
void default_print(char *s, void *v) {
    printf("[%s:%p]", s, v);
}