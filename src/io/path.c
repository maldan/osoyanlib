#include "../../include/io/path.h"

char *path_file_basename(char *path) {
#ifdef __MINGW32__
    EQU_STRING_ARRAY(X) = chars_split(path, "\\", 0);
#else
    EQU_STRING_ARRAY(X) = chars_split(path, "/", 0);
#endif
    char *pathOut = chars_clone(X->list[X->length - 1]->list);
    DESTROY_STRING_ARRAY(X)
    return pathOut;
}
