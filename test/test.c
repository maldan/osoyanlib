#include "../osoyan.h"

int main() {
    MEMORY_INIT;

    NEW_STRING(X);
    string_add(X, "hello");
    string_add(X, "world");
    assert(strcmp(X->list, "helloworld") == 0);

    return 0;
}