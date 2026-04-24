#include "csv.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Table {
    char*** data;
    int rows;
    int cols;
    int* widths;
};

static bool checkTable(Table* t, int expectedRows, int expectedCols, const char* expectedData[][10])
{
    if (t->rows != expectedRows || t->cols != expectedCols) {
        return false;
    }
    for (int i = 0; i < expectedRows; i++) {
        for (int j = 0; j < expectedCols; j++) {
            if (strcmp(t->data[i][j], expectedData[i][j]) != 0) {
                return false;
            }
        }
    }
    return true;
}

static bool checkOutput(Table* t, const char* expected)
{
    char outName[] = "/tmp/csvTestOutXXXXXX";
    int fd = mkstemp(outName);
    if (fd == -1) {
        return false;
    }
    close(fd);

    FILE* out = fopen(outName, "w");
    if (!out) {
        return false;
    }

    printTable(t, out);
    fclose(out);

    FILE* in = fopen(outName, "r");
    if (!in) {
        remove(outName);
        return false;
    }
    char buf[4096] = "";
    char line[512];
    while (fgets(line, sizeof(line), in)) {
        strncat(buf, line, sizeof(buf) - strlen(buf) - 1);
    }
    fclose(in);
    remove(outName);

    return strcmp(buf, expected) == 0;
}

static char* makeTempCSV(const char* data)
{
    char name[] = "/tmp/csvXXXXXX";
    int fd = mkstemp(name);
    if (fd == -1) {
        return NULL;
    }
    close(fd);

    FILE* f = fopen(name, "w");
    fprintf(f, "%s", data);
    fclose(f);
    return strdup(name);
}

static void deleteTemp(const char* name)
{
    if (name) {
        remove(name);
        free((void*)name);
    }
}

bool testCorrect(void)
{
    const char* csv1 = "Test1,Test2\ntest,123\ntestLongString,72.5\nother,3\n";
    char* f1 = makeTempCSV(csv1);
    Table* t = createTable();
    int res = readCSV(t, f1);
    deleteTemp(f1);

    if (res != 0) {
        printf("Ошибка 1: readCSV вернул %d\n", res);
        return false;
    }

    const char* expectedData[10][10] = {
        { "Test1", "Test2" },
        { "test", "123" },
        { "testLongString", "72.5" },
        { "other", "3" }
    };
    if (!checkTable(t, 4, 2, expectedData)) {
        printf("Ошибка: checkTable\n");
        return false;
    }
    if (t->widths[0] != 14 || t->widths[1] != 5) {
        printf("Ошибка: widths %d, %d\n", t->widths[0], t->widths[1]);
        return false;
    }

    const char* expectedOut = "+================+=======+\n"
                              "| Test1          | Test2 |\n"
                              "+================+=======+\n"
                              "| test           |   123 |\n"
                              "+----------------+-------+\n"
                              "| testLongString |  72.5 |\n"
                              "+----------------+-------+\n"
                              "| other          |     3 |\n"
                              "+----------------+-------+\n";
    if (!checkOutput(t, expectedOut)) {
        printf("Ошибка: checkOutput\n");
        return false;
    }

    deleteTable(t);
    printf("Этап 1 пройден успешно\n");

    const char* csv2 = "";
    char* f2 = makeTempCSV(csv2);
    Table* t2 = createTable();
    res = readCSV(t2, f2);
    deleteTemp(f2);

    if (res != 0) {
        printf("Ошибка 2: readCSV пустой файл\n");
        return false;
    }

    if (t2->rows != 0 || t2->cols != 0) {
        printf("Ошибка: пустой файл вернул rows=%d, cols=%d\n", t2->rows, t2->cols);
        return false;
    }

    deleteTable(t2);
    printf("Этап 2 пройден успешно\n");
    return true;
}

bool testIncorrect(void)
{
    Table* t = createTable();
    int res = readCSV(t, "/tmp/nonExistFile.csv");
    if (res == 0) {
        deleteTable(t);
        return false;
    }
    deleteTable(t);

    printLine(NULL, NULL, 0, '-');
    deleteTable(NULL);

    Table* t2 = createTable();
    computeWidths(t2);
    deleteTable(t2);

    const char* csv = "A,BB\n\"3456, banana\",twix";
    char* file = makeTempCSV(csv);
    if (!file) {
        return false;
    }
    Table* t3 = createTable();
    res = readCSV(t3, file);
    deleteTemp(file);
    if (res != 0) {
        deleteTable(t3);
        return false;
    }
    if (t3->rows != 2 || t3->cols != 3) {
        deleteTable(t3);
        return false;
    }
    if (strcmp(t3->data[1][0], "\"3456") != 0 || strcmp(t3->data[1][1], " banana\"") != 0) {
        deleteTable(t3);
        return false;
    }
    deleteTable(t3);

    return true;
}

int main(void)
{
    if (!testCorrect()) {
        printf("Падает testCorrect\n");
        return 1;
    }
    if (!testIncorrect()) {
        printf("Падает testIncorrect\n");
        return 1;
    }
    printf("Все тесты пройдены\n");
    return 0;
}
