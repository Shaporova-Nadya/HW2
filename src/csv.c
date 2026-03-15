#include "csv.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 8192

int isNumeric(const char* s)
{
    if (!s || !*s) {
        return 0;
    }
    char* endptr;

    strtod(s, &endptr);

    if (s == endptr)
        return 0;

    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }

    return *endptr == '\0';
}

void printLine(FILE* out, int* widths, int cols, char fill)
{
    fprintf(out, "+");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < widths[i] + 2; j++) {
            fputc(fill, out);
        }
        fprintf(out, "+");
    }
    fprintf(out, "\n");
}