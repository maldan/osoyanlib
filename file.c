#include "file.h"

int file_get_contents(const char *path, char **buffer, size_t *length) {
    FILE *file = fopen(path, "rb");

    if (!file)
        return 0;

    // Get file length
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Create buffer
    *buffer = malloc(*length + 1);
    memset(*buffer, 0, *length + 1);
    fread(*buffer, 1, *length, file);

    // Close file
    fclose(file);

    return 1;
}

int file_put_contents(const char *path, char *buffer, size_t length) {
    FILE *file = fopen(path, "wb");

    if (!file)
        return 0;

    // Start of file
    fseek(file, 0, SEEK_SET);

    if (length <= 0) length = strlen(buffer);

    // Create buffer
    fwrite(buffer, length, 1, file);

    // Close file
    fflush(file);
    fclose(file);

    return 1;
}

char *file_get_path(char *fullPath) {
    /*char **out;
    size_t length;
    chars_split(fullPath, "/", &out, &length, 0);

    char *finalPathOut = calloc(1, strlen(fullPath) + 1);
    for (int i = 0; i < length - 1; ++i) {
        strcat(finalPathOut, out[i]);
        strcat(finalPathOut, "/");
    }

    return finalPathOut;*/
    return fullPath;
}

char *file_get_filename(char *fullPath) {
    /*char **out;
    size_t length;
    chars_split(fullPath, "/", &out, &length, 0);

    char *finalPathOut = calloc(1, strlen(out[length - 1]) + 1);
    strcat(finalPathOut, out[length - 1]);

    return finalPathOut;*/
    return fullPath;
}