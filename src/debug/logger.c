#include "../../include/debug/logger.h"


/* Standard file descriptors.  */
#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */

void logger_log(const char *fileName, size_t line, const char *format, ...) {
    // Get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // Get tile Time
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Get file name
    fileName = basename((char *)fileName);

    // Create string for output
    NEW_STRING(string);
    va_list argPtr;
    va_start(argPtr, format);

    // Measure string length
    size_t length = vsnprintf(NULL, 0, format, argPtr);
    char *str = MEMORY_ALLOCATE(length + 1);

    // Add formatted string
    va_end(argPtr);
    va_start(argPtr, format);
    vsprintf(str, format, argPtr);
    string_put(string, str);
    MEMORY_FREE(str);
    va_end(argPtr);

    // Terminal is too small
    if (w.ws_col < 64) {
        printf("[INFO] %2.d:%2.d:%2.d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        printf("%s:%zu\n", fileName, line);
        printf("%s\n", string->list);
        printf("─────\n");
        DESTROY_STRING(string);
        return;
    }

    // Add padding
    char *leftPad = "                 ";
    printf(ANSI_COLOR_YELLOW);
    printf("[INFO] %2.d:%2.d:%2.d  ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    printf(ANSI_COLOR_RESET);

    size_t used = 17;
    size_t maxStringLength = w.ws_col - strlen(fileName) - 21 - 4;
    struct String *slicedString = string_break(string, maxStringLength - 2);
    DESTROY_STRING(string);
    string = slicedString;

    struct StringArray *tuples = chars_split(string->list, "\n", 0);

    for (size_t i = 0; i < tuples->length; ++i) {
        char stringCopy[tuples->list[i]->length + 1];
        MEMORY_COPY(stringCopy, tuples->list[i]->list, tuples->list[i]->length + 1, stringCopy, tuples->list[i]->allocated);
        if (tuples->list[i]->length > maxStringLength) {
            stringCopy[maxStringLength - 3] = '.';
            stringCopy[maxStringLength - 2] = '.';
            stringCopy[maxStringLength - 1] = '.';
            stringCopy[maxStringLength] = 0;
        }

        if (i > 0) printf("%s│  %s", leftPad, stringCopy);
        else printf("│  %s", stringCopy);
        used += strnlen(stringCopy, USHRT_MAX) + 3;

        if (i == 0) {
            int32_t spaces = w.ws_col - strnlen(fileName, 16384) - 4 - used;
            // printf("[%zu]", spaces);
            if (spaces > 0) {
                char gas[spaces + 1];

                // printf("SSSS %d sSSS", spaces);

                memset(gas, 0, spaces + 1);
                memset(gas, ' ', spaces);
                printf(ANSI_COLOR_GREEN);
                printf("%s%s:%zu", gas, fileName, line);
                printf(ANSI_COLOR_RESET);
            }
        }

        printf("\n");
    }

    int32_t size = (w.ws_col - strlen(leftPad)) * 3;
    // printf("[%d]", size);
    char *s = MEMORY_ALLOCATE(size + 1);

    for (size_t j = 0; j < w.ws_col - strlen(leftPad) - 1; ++j) {
        sprintf(s + strlen(s), "─");
    }

    MEMORY_FREE(s);

    // wchar_t wchar = "─";
    // wmemset(s, wchar, w.ws_col - strlen(leftPad) - 1);
    // memset(s, '_', w.ws_col - strlen(leftPad) - 1);
    printf("%s├%s\n", leftPad, s);

    DESTROY_STRING_ARRAY(tuples);
    DESTROY_STRING(string);
    // printf("[INFO] %2.d:%2.d:%2.d  %s                       %s:%zu\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, string->list, fileName, line);
}
