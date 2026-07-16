#ifndef EDITOR_H
#define EDITOR_H

typedef struct {
    char *chars;
    int len;
} erow;

void ed_open(const char *filename);
void ed_append_row(char *line, int len);

#endif