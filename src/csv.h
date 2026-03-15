#pragma once
#include <stdio.h>

#define MAX_LINE 8192

// isNumeric checks if a string is a number
int isNumeric(const char* s);

// printLine draws pseudographics
void printLine(FILE* out, const int* widths, int cols, char fill);