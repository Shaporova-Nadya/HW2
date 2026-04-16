#include "csv.h"

#include <stdlib.h>

int main(void)
{
    Table* table = createTable();
    if (!table) {
        return -1;
    }

    if (readCSV(table, "input.csv") != 0) {
        deleteTable(table);
        return -1;
    }

    FILE* out = fopen("output.txt", "w");
    if (!out) {
        deleteTable(table);
        return -1;
    }

    printTable(table, out);

    fclose(out);
    deleteTable(table);
    return 0;
}