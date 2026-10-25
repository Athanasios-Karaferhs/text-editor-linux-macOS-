# C Terminal Text Editor

A lightweight, terminal-based text editor built from scratch in C. This project implements low-level POSIX terminal handling to switch to raw mode, uses a custom dynamic buffer structure for fast text editing, and includes robust file I/O for reading and writing files.

---

## Features

- **File I/O**: Open existing text files from the command line and save edits back to disk.
- **Text Editing**: Full character insertion/deletion, along with line splitting (`Enter`) and merging (`Backspace`).
- **Cursor Navigation**: Seamless movement using standard arrow keys, complete with edge and bound checking.
- **Dynamic Terminal Resizing**: Handles live `SIGWINCH` signal changes to re-render the editor on terminal resize.
- **Raw Mode Terminal Control**: Disables canonical mode and echo using `termios` to process keypresses byte-by-byte.

---

## Requirements & Building

A C compiler (such as `gcc` or `clang`) and a POSIX-compliant environment (Linux, macOS, or WSL on Windows) are required.

### Build

Compile the source files using `gcc`:

```bash
gcc terminal.c editor.c -o terminal -lncurses
```
