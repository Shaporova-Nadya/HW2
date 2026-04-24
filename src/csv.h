#pragma once
#include <stdio.h>

#define MAX_LINE 8192

typedef struct Table Table;

// isNumeric checks if a string is a number
int isNumeric(const char* s);

// printLine draws pseudographics
void printLine(FILE* out, const int* widths, int cols, char fill);

// creates table
Table* createTable(void);

// deletes table
void deleteTable(Table* t);

// calculates widths
void computeWidths(Table* t);

// reads the CSV file
int readCSV(Table* t, const char* filename);

// prints the table
void printTable(const Table* t, FILE* out);
void printLine(FILE* out, const int* widths, int cols, char fill);
