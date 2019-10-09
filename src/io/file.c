#include "../../include/io/file.h"
#include "../../include/io/print.h"

struct FileInfo *file_get_info(const char *path, bool includeContent) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "File \"%s\" not found\n", path);
        return 0;
    }

    struct FileInfo *fileInfo = MEMORY_ALLOCATE_STRUCT(FileInfo);
    CHARS_COPY_TO(fileInfo->path, path);
    fileInfo->fileName = basename((char *)path);

    // Get file length
    fseek(file, 0, SEEK_END);
    fileInfo->size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Create buffer
    if (includeContent) {
        fileInfo->data = MEMORY_ALLOCATE(fileInfo->size + 1);
        memset(fileInfo->data, 0, fileInfo->size + 1);
        fread(fileInfo->data, 1, fileInfo->size, file);
    }

    // Close file
    fclose(file);

    return fileInfo;
}

struct Blob *file_get_slice_of_contents(const char *path, size_t start, size_t length) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "File \"%s\" not found\n", path);
        return 0;
    }

    // Get file size
    size_t fileLength = 0;
    fseek(file, 0, SEEK_END);
    fileLength = length; // ftell(file);
    fseek(file, start, SEEK_SET);

    // Get file data
    NEW_BLOB(fileData)
    blob_allocate(fileData, fileLength + 1);
    fread(fileData->list, 1, fileLength, file);
    fileData->position = fileLength;
    fileData->length = fileLength;

    // Close file
    fclose(file);

    return fileData;
}

struct Blob * file_get_contents(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "File \"%s\" not found\n", path);
        return 0;
    }

    // Get file size
    size_t fileLength = 0;
    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Get file data
    NEW_BLOB(fileData)
    blob_allocate(fileData, fileLength + 1);
    fread(fileData->list, 1, fileLength, file);
    fileData->position = fileLength;
    fileData->length = fileLength;

    // Close file
    fclose(file);

    return fileData;
}

bool file_put_contents(const char *path, void *buffer, size_t length) {
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

size_t file_get_size(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "File \"%s\" not found\n", path);
        return 0;
    }

    // Get file size
    size_t fileLength = 0;
    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    return fileLength;
}

bool file_put_blob(const char *path, struct Blob *blob) {
    return file_put_contents(path, blob->list, blob->length);
}

bool file_put_bitset(const char *path, struct BitSet *bitSet) {
    return file_put_contents(path, bitSet->list, bitSet->length / 8);
}

#ifdef __MINGW32__
void ____file_real_search(struct Vector *fileInfoList, const char *path, regex_t *filter, size_t flags) {
    puts("Not working in windows");
    exit(1);
}
#else
void ____file_real_search(struct Vector *fileInfoList, const char *path, regex_t *filter, size_t flags) {
    DIR *d = opendir(path);
    struct dirent *dir;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;

            if (dir->d_type == DT_REG) {
                struct FileInfo *fileInfo = MEMORY_ALLOCATE_STRUCT(FileInfo);

                // Create new path
                NEW_STRING(newPath);
                string_put(newPath, path);
                if (path[strlen(path) - 1] != '/') string_put(newPath, "/");
                string_put(newPath, dir->d_name);

                // If using filter
                if (filter) {
                    int reti = regexec(filter, basename(newPath->list), 0, NULL, 0);
                    if (reti) {
                        // Destroy old copy
                        DESTROY_STRING(newPath);
                        MEMORY_FREE(fileInfo);
                        continue;
                    }
                }

                // Clone path int fileInfo->path
                CHARS_COPY_TO(fileInfo->path, newPath->list);

                // Define file name and data
                fileInfo->fileName = basename(fileInfo->path);

                if (flags == FILE_INFO_INCLUDE_DATA) {
                    NEW_FILE_INFO(XX, fileInfo->path);
                    fileInfo->data = XX->data;
                    fileInfo->size = XX->size;
                    MEMORY_FREE(XX->path);
                    MEMORY_FREE(XX);
                } else {
                    fileInfo->data = MEMORY_ALLOCATE(0);
                }

                // Destroy old copy
                DESTROY_STRING(newPath);

                // Vector push
                vector_push(fileInfoList, fileInfo);
            }

            if (dir->d_type == DT_DIR) {
                char curPath[1024];
                memset(curPath, 0, 1024);
                sprintf(curPath + strlen(curPath), "%s", path);
                sprintf(curPath + strlen(curPath), "%s/", dir->d_name);
                ____file_real_search(fileInfoList, curPath, filter, flags);
            }
        }

        closedir(d);
    } else {
        printf("Can't open dir %s", path);
        exit(1);
    }
}
#endif


struct Vector * file_search(const char *path, const char *filter, size_t flags) {
#ifdef __MINGW32__
    puts("Not working in windows");
    exit(1);
#else
    NEW_VECTOR(X, struct FileInfo)

    // Compile regex
    regex_t filterRegex;
    if (filter) {
        int reti = regcomp(&filterRegex, filter, REG_EXTENDED);
        if (reti) {
            fprintf(stderr, "Could not compile regex %s\n", filter);
            exit(1);
        }
    }

    // Search
    if (filter) ____file_real_search(X, path, &filterRegex, flags);
    else ____file_real_search(X, path, 0, flags);

    // Free regex
    regfree(&filterRegex);

    return X;
#endif
}

bool file_copy(char *fromPath, char *toPath) {
    EQU_BLOB(X) = file_get_contents(fromPath);
    if (!X) return false;
    if (!file_put_blob(toPath, X)) {
        DESTROY_BLOB(X)
        return false;
    }
    DESTROY_BLOB(X)
    return true;
}

