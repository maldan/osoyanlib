#include <pwd.h>
#include "../../include/util/os.h"
#include "../../include/string/string.h"

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
