#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

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
    // raw.c_lflag &= ~(ECHO | ICANON); // Example of turning off echo/canonical mode
    // tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}