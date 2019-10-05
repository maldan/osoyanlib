#include "../../include/debug/console.h"

/* Standard file descriptors.  */
#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */

struct termios saved_attributes;

struct winsize console_get_window_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w;
}

void console_restore_terminal_mode(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

void console_non_canonical_mode() {
    struct termios tattr;
    char *name;

    /* Make sure stdin is a terminal. */
    if (!isatty(STDIN_FILENO)) {
        fprintf(stderr, "Not a terminal.\n");
        exit(EXIT_FAILURE);
    }

    /* Save the terminal attributes so we can restore them later. */
    tcgetattr(STDIN_FILENO, &saved_attributes);
    atexit(console_restore_terminal_mode);

    /* Set the funny terminal modes. */
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= (~ICANON & ~ECHO); /* Clear ICANON and ECHO. */
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    // tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
    tcsetattr(0,TCSANOW, &tattr);
}

void console_fill_screen(char chr) {
    struct winsize size = console_get_window_size();
    printf("%s", "\x1b[H");
    for (size_t i = 0; i < size.ws_row; ++i) {
        for (size_t j = 0; j < size.ws_col; ++j) {
            printf("%c", chr);
        }
    }
}
