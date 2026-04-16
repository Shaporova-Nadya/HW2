#pragma once
#include <stdio.h>

#define MAX_LINE 8192

typedef struct Table {
    char*** data;
    int rows;
    int cols;
    int* widths;
} Table;

// isNumeric checks if a string is a number
int isNumeric(const char* s);

// printLine draws pseudographics
void printLine(FILE* out, const int* widths, int cols, char fill);

// creates table
Table* createTable(void);

// deletes table
void deleteTable(Table* t);

// reads all lines from file
static char** readLines(const char* filename, int* outRowCount);

// determines the max number of cols
static int getMaxCols(char** lines, int rowCount);

// fills the table from an array of rows
static void fillTable(Table* t, char** lines, int rowCount, int maxCols);

// calculates widths
void computeWidths(Table* t);

// reads the CSV file
int readCSV(Table* t, const char* filename);

// prints the table
void printTable(const Table* t, FILE* out);