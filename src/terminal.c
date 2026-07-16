#define _POSIX_C_SOURCE 200809L // stack overflow cluch with this one! δεν εχω ιδεα τι κανει αλαλ δουλευει:) (ο τυπος μ ειπε οτι πειραζει τα h files ιδκ πως)

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h> // για SIGWINCH
#define UPPER_ARROW_KEY 1000
#define DOWN_ARROW_KEY 1001
#define RIGHT_ARROW_KEY 1002
#define LEFT_ARROW_KEY 1003
#define CTRL_KEY(k) ((k) & 0x1f) /*κανουμε ενα bitwise and (αχ κυριε δημητριου). AND με το q και το 0x1f δινει 17 κ μετα παει κ ελεγχει \
αντι c == 17 (ctrl+q)*/
#include <sys/ioctl.h>           //για να μετραμε ποσα rows/col εχει το terminal
#include <errno.h>               //resize
#include "editor.h"

void enable_raw_mode(void);
int read_key(void); // gt to read_key einai xamhla
int get_window_size(int *, int *);
void refresh_screen(int, int, int);
void handle_sigwinch(int);

// Global variable to store original settings so atexit can access it
struct termios orig_termios;

volatile sig_atomic_t window_resized = 1; // flag για terminal resize

void handle_sigwinch(int sig)
{
    (void)sig;
    window_resized = 1;
}

int main(int argc, char *argv[]) /*argc= παιρνει αριθμο απο arguments ενω το argv εκτυπωνει το αλφαριθμιτικο*/
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    ed_open(argv[1]);

    int rows, cols;
    int c;
    int key_pressed = 0;

    int cx = 0, cy = 0;
    ed_open("text.txt");
    enable_raw_mode();

    // όταν αλλάζει το μέγεθος του terminal στέλνει SIGWINCH
    struct sigaction sa;
    sa.sa_handler = handle_sigwinch;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, NULL);

    while (1)
    {
        // το ioctl τρέχει μόνο όταν αλλάξει το terminal
        if (window_resized)
        {
            window_resized = 0;
            get_window_size(&rows, &cols);
        }

        refresh_screen(key_pressed, cx, cy);

        c = read_key();
        if (c == CTRL_KEY('q'))
            break;

        else
        {
            key_pressed = c;

            if (c == UPPER_ARROW_KEY)
            {
                if (cy > 0)
                    cy--;
            }

            else if (c == DOWN_ARROW_KEY)
            {
                if (cy < rows - 1)
                    cy++;
            }

            else if (c == RIGHT_ARROW_KEY)
            {
                if (cx < cols - 1)
                    cx++;
            }

            else if (c == LEFT_ARROW_KEY)
            {
                if (cx > 0)
                    cx--;
            }
        }
    }
    write(STDOUT_FILENO, "\x1b[2J", 4); // Πριν κλεισω το προγραμμα κανω clear και παω στο 0,0
    write(STDOUT_FILENO, "\x1b[H", 3);
    return 0;
}

// Restores terminal
void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// Initializes the terminal and registers the cleanup handler
void enable_raw_mode()
{
    // tcgetattr reads the current settings into a struct
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
        perror("tcgetattr failed");
        exit(EXIT_FAILURE);
    }

    if (atexit(disable_raw_mode) != 0)
    {
        perror("atexit registration failed");
        exit(EXIT_FAILURE);
    }

    // Modify flags here(το εκανα)
    struct termios raw = orig_termios;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    /*στα flags καθε bit κανει reprisent διαφορετικο behavior στο terminal.*/

    // tcsetattr pushes a struct you've prepared back onto the terminal
    //(αντιθετο το tcgetattr)

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        perror("tcsetattr failed");
        exit(EXIT_FAILURE);
    }
}

int read_key()
{
    char c;
    int n;

    while ((n = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (n == -1)
        {
            if (errno == EINTR)
                continue; // retry
            perror("read failed");
            exit(EXIT_FAILURE);
        }

        // n == 0  timeout kanei loop k try again
    }
    if (c != 27)
    {
        return c;
    }

    char seq[2];

    if (read(STDIN_FILENO, &seq[0], 1) != 1)
        return 27;

    if (read(STDIN_FILENO, &seq[1], 1) != 1)
        return 27;

    if (seq[0] == '[')
    {

        if (seq[1] == 'A')
            return UPPER_ARROW_KEY;

        if (seq[1] == 'B')
            return DOWN_ARROW_KEY;

        if (seq[1] == 'C')
            return RIGHT_ARROW_KEY;

        if (seq[1] == 'D')
            return LEFT_ARROW_KEY;
    }

    return 27;
}

int get_window_size(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
        return -1;

    *cols = ws.ws_col;
    *rows = ws.ws_row;

    return 0;
}

void refresh_screen(int key_pressed, int cx, int cy)
{
    write(STDOUT_FILENO, "\x1b[2J", 4); // clear
    write(STDOUT_FILENO, "\x1b[H", 3);  // ξεκιναμε απο το 1,1
    printf("TEST: %d ('%c') \r\n", key_pressed, key_pressed);

    // μεταφα τα col+1,row+1 σε εναν μικρο πινακα buffer κ μεταφερετε ο κερσορας στο κατακυλο σημειο rows κ cols.
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", cy + 1, cx + 1);
    write(STDOUT_FILENO, buf, strlen(buf));
}