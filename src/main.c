#include "csv.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE* in = fopen("input.csv", "r");
    if (in == NULL) {
        return -1;
    }

    char line[MAX_LINE];
    int* widths = NULL;
    int colCount = 0;

    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\n\r")] = 0;
        int current_col = 0;
        char* ptr = line;

        while (1) {
            char* comma = strchr(ptr, ',');
            int len = comma ? (int)(comma - ptr) : (int)strlen(ptr);

            if (current_col >= colCount) {
                widths = realloc(widths, sizeof(int) * (current_col + 1));
                widths[current_col] = 0;
                colCount = current_col + 1;
            }

            if (len > widths[current_col])
                widths[current_col] = len;

            if (!comma)
                break;
            ptr = comma + 1;
            current_col++;
        }
    }

    rewind(in);
    FILE* out = fopen("output.txt", "w");
    int rowIdx = 0;

    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\n\r")] = 0;

        if (rowIdx == 0)
            printLine(out, widths, colCount, '=');

        fprintf(out, "|");
        char* ptr = line;
        for (int i = 0; i < colCount; i++) {
            char cell[MAX_LINE] = "";
            char* comma = strchr(ptr, ',');
            if (comma) {
                strncpy(cell, ptr, comma - ptr);
                cell[comma - ptr] = '\0';
                ptr = comma + 1;
            } else {
                strcpy(cell, ptr);
                ptr += strlen(ptr);
            }

            if (rowIdx == 0)
                fprintf(out, " %-*s |", widths[i], cell);
            else if (isNumeric(cell))
                fprintf(out, " %*s |", widths[i], cell);
            else
                fprintf(out, " %-*s |", widths[i], cell);
        }
        fprintf(out, "\n");

        if (rowIdx == 0)
            printLine(out, widths, colCount, '=');
        else
            printLine(out, widths, colCount, '-');
        rowIdx++;
    }

    fclose(in);

    fclose(out);

    free(widths);
    return 0;
}
