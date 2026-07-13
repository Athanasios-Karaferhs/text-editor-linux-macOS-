#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#define UPPER_ARROW_KEY 1000
#define DOWN_ARROW_KEY 1001
#define RIGHT_ARROW_KEY 1002
#define LEFT_ARROW_KEY 1003

void enable_raw_mode(void);
int read_key(void); // gt to read_key einai xamhla

int main()
{
    enable_raw_mode();
    int c; // int γτ εχει values πανω του 1000
    while ((c = read_key()) != 'q')
    {
        if (c == UPPER_ARROW_KEY)
        {
            printf("UP\r\n");
        }
        else if (c == DOWN_ARROW_KEY)
        {
            printf("DOWN\r\n");
        }
        else if (c == RIGHT_ARROW_KEY)
        {
            printf("RIGHT\r\n");
        }
        else if (c == LEFT_ARROW_KEY)
        {
            printf("LEFT\r\n");
        }
        else if (iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else
        {
            printf("%d ('%c')\r\n", c, c);
        }
    }
}

// Global variable to store original settings so atexit can access it
struct termios orig_termios;

// Restores the terminal to its original state
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

    // Register the cleanup function to run automatically at exit
    if (atexit(disable_raw_mode) != 0)
    {
        perror("atexit registration failed");
        exit(EXIT_FAILURE);
    }

    // SOS !! Modify flags here
    struct termios raw = orig_termios;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    /*στα flags καθε bit κανει reprisent διαφορετικο behavior στο terminal.*/
    // tcsetattr pushes a struct you've prepared back onto the terminal(αντιθετο το tcgetattr)
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        perror("tcgetattr failed");
        exit(EXIT_FAILURE);
    }
}

int read_key(void)
{
    char c;
    int n;

    while ((n = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (n == -1)
        {
            perror("read failed");
            exit(EXIT_FAILURE);
        }
        // n == 0 shmainei timeout kanei loop k try again
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