#include "../../include/util/os.h"

#ifdef __MINGW32__
char *os_home_dir(char *addPath) {
    puts("Not working in winodws");
    exit(1);
    return "sas";
}
#else
char *os_home_dir(char *addPath) {
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;

    if (!addPath)
        return homedir;

    NEW_STRING(X)
    string_put(X, homedir);
    string_put(X, "/");
    string_add(X, addPath);

    char *finalPath = X->list;
    MEMORY_FREE(X)

    return finalPath;
}
#endif

void os_sleep(int ms) {
#ifdef __MINGW32__
    Sleep(ms);
#else
    usleep(ms);
#endif
}
