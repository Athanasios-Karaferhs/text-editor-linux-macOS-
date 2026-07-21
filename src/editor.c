#define _POSIX_C_SOURCE 200809L // για να εχουμε το geline()
#include "editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char status_msg[64] = "";
erow *rows = NULL;
int num_rows = 0;

int ed_open(const char *filename)
{

    FILE *input_f;
    input_f = fopen(filename, "r"); // opens the file in read mode
    if (input_f != NULL)
    {
        char *line = NULL;
        size_t linecap = 0;
        ssize_t linelen;

        while ((linelen = getline(&line, &linecap, input_f)) != -1) // καλει την getline() και κανει Store οτι υπαρχει στην linelen
        {
            while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
            {
                linelen--;
            }
            ed_append_row(line, linelen);
        }

        free(line);
        fclose(input_f);

        return 1;
    }
    else
    {
        perror("Can not open the file.");
        return 0;
    }
}

void ed_append_row(char *line, int len)
{
    rows = realloc(rows, sizeof(erow) * (num_rows + 1)); // δωσε ενα παραπανω slot
    rows[num_rows].chars = malloc(len + 1);              //+1 για το \0 επισησ .char κανω access στο erow struct+ με το malloc εκανα alocate στο νεο τσανκ μνημης
    memcpy(rows[num_rows].chars, line, len);             // κανω move το text στο νεο σημειο στην μνημη
    rows[num_rows].chars[len] = '\0';
    rows[num_rows].len = len;
    num_rows++;
}

void ed_instert_char(int row_index, int col_index, char c)
{
    // θελω να κανω ολισθηση προς τα δεξια για να βαλω εναν νεο χαρακτηρα
    rows[row_index].chars = realloc(rows[row_index].chars, rows[row_index].len + 2);
    // θελω μνημη στο memmove για αυτο βαζω &
    memmove(&rows[row_index].chars[col_index + 1], // εδω θα παει ο νεος χαρακτηρας μετα την ολισθηση
            &rows[row_index].chars[col_index],
            rows[row_index].len - col_index + 1); // το col_index δινχει που θελω να βαλω τον νεο χαρακτηρα

    rows[row_index].chars[col_index] = c;
    rows[row_index].len++;
}

void ed_insert_row(int index, char *text, int len)
{
    rows = realloc(rows, sizeof(erow) * (num_rows + 1));

    memmove(&rows[index + 1], &rows[index], sizeof(erow) * (num_rows - index));

    rows[index].chars = malloc(len + 1);
    memcpy(rows[index].chars, text, len);
    rows[index].chars[len] = '\0';
    rows[index].len = len;

    num_rows++;
}

void ed_delete_char(int row_index, int col_index)
{
    memmove(&rows[row_index].chars[col_index - 1],
            &rows[row_index].chars[col_index],
            rows[row_index].len - col_index + 1);

    rows[row_index].len--;
}

void ed_save(const char *filename)
{
    FILE *output_f;
    output_f = fopen(filename, "w"); // w for writing

    if (output_f != NULL)
    {
        for (int i = 0; i < num_rows; i++)
            fprintf(output_f, "%s\n", rows[i].chars);
        snprintf(status_msg, sizeof(status_msg), "Saved!");
        fclose(output_f);
    }
    else
        perror("Can not save the file.");
}