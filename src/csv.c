#include "csv.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 8192

typedef struct Table {
    char*** data;
    int rows;
    int cols;
    int* widths;
} Table;

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

void printLine(FILE* out, const int* widths, int cols, char fill)
{
    if (!out || (cols > 0 && !widths)) {
        return;
    }

    fprintf(out, "+");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < widths[i] + 2; j++) {
            fputc(fill, out);
        }
        fprintf(out, "+");
    }
    fprintf(out, "\n");
}

Table* createTable(void)
{
    Table* t = malloc(sizeof(Table));
    if (!t) {
        return NULL;
    }

    t->data = NULL;
    t->rows = 0;
    t->cols = 0;
    t->widths = NULL;
    return t;
}

void deleteTable(Table* t)
{
    if (!t) {
        return;
    }

    for (int i = 0; i < t->rows; i++) {
        for (int j = 0; j < t->cols; j++) {
            free(t->data[i][j]);
        }
        free(t->data[i]);
    }

    free(t->data);
    free(t->widths);
    free(t);
}

static char** readLines(const char* filename, int* outRowCount)
{
    FILE* in = fopen(filename, "r");
    if (!in) {
        return NULL;
    }

    char** lines = NULL;
    int count = 0;
    char buf[MAX_LINE];

    while (fgets(buf, sizeof(buf), in)) {
        buf[strcspn(buf, "\r\n")] = '\0';
        char* line = strdup(buf);
        if (!line) {
            for (int i = 0; i < count; i++) {
                free(lines[i]);
            }
            free(lines);
            fclose(in);
            return NULL;
        }
        char** tmp = realloc(lines, (count + 1) * sizeof(char*));
        if (!tmp) {
            free(line);
            for (int i = 0; i < count; i++) {
                free(lines[i]);
            }
            free(lines);
            fclose(in);
            return NULL;
        }
        lines = tmp;
        lines[count++] = line;

        if (strlen(buf) == MAX_LINE - 1 && buf[MAX_LINE - 2] != '\n') {
            int ch = fgetc(in);
            while (ch != '\n' && ch != EOF) {
                ch = fgetc(in);
            }
        }
    }
    fclose(in);
    *outRowCount = count;
    if (count == 0) {
        free(lines);
        return malloc(sizeof(char*) * 1);
    }
    return lines;
}

static int getMaxCols(char** lines, int rowCount)
{
    int maxCols = 0;
    for (int i = 0; i < rowCount; i++) {
        int cols = 1;
        for (const char* p = lines[i]; *p; p++) {
            if (*p == ',') {
                cols++;
            }
        }
        if (cols > maxCols) {
            maxCols = cols;
        }
    }
    return maxCols;
}

static void fillTable(Table* t, char** lines, int rowCount, int maxCols)
{
    t->rows = rowCount;
    t->cols = maxCols;

    if (rowCount == 0 || maxCols == 0) {
        t->data = NULL;
        return;
    }

    t->data = malloc(rowCount * sizeof(char**));
    if (!t->data) {
        return;
    }

    for (int i = 0; i < rowCount; i++) {
        t->data[i] = malloc(maxCols * sizeof(char*));
        if (!t->data[i]) {
            for (int k = 0; k < i; k++) {
                for (int j = 0; j < maxCols; j++) {
                    free(t->data[k][j]);
                }
                free(t->data[k]);
            }
            free(t->data);
            t->data = NULL;
            t->rows = 0;
            return;
        }
        for (int j = 0; j < maxCols; j++) {
            t->data[i][j] = NULL;
        }

        const char* ptr = lines[i];
        int col = 0;
        while (col < maxCols) {
            const char* comma = strchr(ptr, ',');
            int len = comma ? (int)(comma - ptr) : (int)strlen(ptr);
            char* field = malloc(len + 1);
            if (!field) {
                for (int k = 0; k <= i; k++) {
                    for (int j = 0; j < maxCols; j++) {
                        free(t->data[k][j]);
                    }
                    free(t->data[k]);
                }
                free(t->data);
                t->data = NULL;
                t->rows = 0;
                return;
            }
            strncpy(field, ptr, len);
            field[len] = '\0';
            t->data[i][col++] = field;
            if (!comma) {
                break;
            }
            ptr = comma + 1;
        }

        while (col < maxCols) {
            t->data[i][col++] = strdup("");
        }
    }
}

void computeWidths(Table* t)
{
    if (!t || t->rows == 0 || t->cols == 0) {
        return;
    }
    free(t->widths);
    t->widths = malloc(t->cols * sizeof(int));
    for (int j = 0; j < t->cols; j++) {
        int max = 0;
        for (int i = 0; i < t->rows; i++) {
            int len = (int)strlen(t->data[i][j]);
            if (len > max) {
                max = len;
            }
        }
        t->widths[j] = max;
    }
}

int readCSV(Table* t, const char* filename)
{
    int rowCount;
    char** lines = readLines(filename, &rowCount);
    if (!lines) {
        return -1;
    }

    int maxCols = getMaxCols(lines, rowCount);
    fillTable(t, lines, rowCount, maxCols);

    for (int i = 0; i < rowCount; i++) {
        free(lines[i]);
    }
    free(lines);

    computeWidths(t);
    return 0;
}

void printTable(const Table* t, FILE* out)
{
    if (!t || !out)
        return;

    printLine(out, t->widths, t->cols, '=');

    for (int i = 0; i < t->rows; i++) {
        fprintf(out, "|");
        for (int j = 0; j < t->cols; j++) {
            const char* cell = t->data[i][j];

            if (i == 0 || !isNumeric(cell)) {
                fprintf(out, " %-*s |", t->widths[j], cell);
            } else {
                fprintf(out, " %*s |", t->widths[j], cell);
            }
        }
        fprintf(out, "\n");
        printLine(out, t->widths, t->cols, i == 0 ? '=' : '-');
    }
}
}
