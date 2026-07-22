#ifndef EDITOR_H
#define EDITOR_H

typedef struct
{
    char *chars;
    int len;
} erow;

extern erow *rows;
extern int num_rows;

typedef struct
{
    int row;
    int col;
    char c;
} action;

extern action *undo_stack;
extern int undo_count;

extern char status_msg[64];
extern char save_smsg[64];

int ed_open(const char *filename);
void ed_append_row(char *line, int len);
void ed_instert_char(int row_index, int col_index, char c);
void ed_insert_row(int index, char *text, int len);
void ed_delete_char(int row_index, int col_index);
void ed_save(const char *filename);
void ed_push_undo(int row, int col, char c);
#endif