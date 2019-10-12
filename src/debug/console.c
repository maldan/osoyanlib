#include "../../include/debug/console.h"

#ifndef __MINGW32__
struct termios saved_attributes;

int console_enable_vt_mode() {
    return 0;
}

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
#else
int console_enable_vt_mode() {
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return 0;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return 0;

    dwMode |= 0x0004;
    if (!SetConsoleMode(hOut, dwMode))
        return 0;
    return 1;
}
struct winsize console_get_window_size() {
    int columns, rows;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    struct winsize win;
    win.ws_col = columns;
    win.ws_row = rows;
    return win;
}
void console_restore_terminal_mode(void) {

}
void console_non_canonical_mode() {

}
void console_fill_screen(char chr) {

}
int console_get_key() {
    int c = getch();
    if (c == 224) {
        switch (getch()) {
            case 72: return CONSOLE_KEY_UP;
            case 80: return CONSOLE_KEY_DOWN;
            case 75: return CONSOLE_KEY_LEFT;
            case 77: return CONSOLE_KEY_RIGHT;
            default:
                return CONSOLE_KEY_DEL;
        }
    }
    if (c == 13) return CONSOLE_KEY_ENTER;
    if (c == 3) return CONSOLE_KEY_CTRL_C;
    if (c == 22) return CONSOLE_KEY_CTRL_V;
    return c;
}
#endif
