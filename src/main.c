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
        int currentCol = 0;
        char* ptr = line;

        while (1) {
            char* comma = strchr(ptr, ',');
            int len = comma ? (int)(comma - ptr) : (int)strlen(ptr);

            if (currentCol >= colCount) {
                void* temp = realloc(widths, sizeof(int) * currentCol);
                if (temp == NULL) {
                    free(widths);
                    fclose(in);
                    return 1;
                }
                widths = temp;
                widths[currentCol] = 0;
                colCount = currentCol + 1;
            }

            if (len > widths[currentCol])
                widths[currentCol] = len;

            if (!comma)
                break;
            ptr = comma + 1;
            currentCol++;
        }
    }

    rewind(in);
    FILE* out = fopen("output.txt", "w");
    if (out = NULL) {
        free(widths);
        fclose(in);
        return -1;
    }
    int rowIdx = 0;

    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\n\r")] = '\0';

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
                int len = (int)strlen(ptr);
                strncpy(cell, ptr, len);
                cell[len] = '\0';
                ptr += len;
            }

            if (rowIdx == 0 || !isNumeric(cell)) {
                fprintf(out, "%-*s |", widths[i], cell);
            } else {
                fprintf(out, "%*s |", widths[i], cell);
            }
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
