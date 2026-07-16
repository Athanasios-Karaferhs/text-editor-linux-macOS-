#define _POSIX_C_SOURCE 200809L // για να εχουμε το geline()
#include "editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

erow *rows = NULL;
int num_rows = 0;

void ed_open(const char *filename)
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
    }
    else
        perror("Can not open the file.");
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