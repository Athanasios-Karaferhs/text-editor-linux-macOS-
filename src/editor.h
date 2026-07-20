#ifndef EDITOR_H
#define EDITOR_H

typedef struct
{
    char *chars;
    int len;
} erow;

extern erow *rows;
extern int num_rows;

void ed_open(const char *filename);
void ed_append_row(char *line, int len);
void ed_instert_char(int row_index, int col_index, char c);
void ed_insert_row(int index, char *text, int len);
void ed_delete_char(int row_index, int col_index);
#endif