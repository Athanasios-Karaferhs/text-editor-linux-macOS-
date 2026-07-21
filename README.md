terminal-based text editor built from scratch in C, implementing raw mode terminal handling, a custom text buffer, and file I/O.

FEATURES:
Open and edit text files
Insert/delete characters, split/merge lines (Enter/Backspace for now)
Save changes back to disk
Arrow-key navigation with proper bounds checking
Live terminal resize handling

BUILD:
gcc terminal.c editor.c -o terminal
RUN:
./terminal <filename>

CONTROLS:
Arrow keys — move cursor
Enter — new line
Backspace — delete character / merge lines
Ctrl-S — save
Ctrl-Q — quit

