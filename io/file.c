#include <libgen.h>
#include "file.h"

struct FileInfo * file_get_contents(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file)
        return 0;

    struct FileInfo *fileInfo = MEMORY_ALLOCATE_STRUCT(FileInfo);
    fileInfo->path = path;
    fileInfo->fileName = basename((char *)path);

    // Get file length
    fseek(file, 0, SEEK_END);
    fileInfo->size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Create buffer
    fileInfo->data = MEMORY_ALLOCATE(fileInfo->size + 1);
    memset(fileInfo->data, 0, fileInfo->size + 1);
    fread(fileInfo->data, 1, fileInfo->size, file);

    // Close file
    fclose(file);

    return fileInfo;
}

void print_file_info(const char *fileName, size_t line, struct FileInfo *data) {
    NEW_STRING(X);

    string_add(X, "FileInfo {\n");
    string_add(X, "    path: %s\"%s\"%s,\n", ANSI_COLOR_GREEN, data->path, ANSI_COLOR_RESET);
    string_add(X, "    fileName: %s\"%s\"%s,\n", ANSI_COLOR_GREEN, data->fileName, ANSI_COLOR_RESET);
    string_add(X, "    size: %s%zu%s\n",  ANSI_COLOR_RED, data->size, ANSI_COLOR_RESET);
    string_add(X, "}\n");

    LOGGER_LOG(fileName, line, X->list);

    DESTROY_STRING(X);
}

bool file_put_contents(const char *path, char *buffer, size_t length) {
    FILE *file = fopen(path, "wb");

    if (!file)
        return false;

    // Start of file
    fseek(file, 0, SEEK_SET);

    if (length <= 0) length = strlen(buffer);

    // Create buffer
    fwrite(buffer, length, 1, file);

    // Close file
    fflush(file);
    fclose(file);

    return true;
}