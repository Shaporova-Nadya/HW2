#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef enum {
    FIND,
    ADD,
    DELETE,
    SAVE,
    QUIT,
    UNKNOWN
} Command;

Command getComand(char* str)
{
    if (strcmp(str, "find") == 0) {
        return FIND;
    }
    if (strcmp(str, "add") == 0) {
        return ADD;
    }
    if (strcmp(str, "delete") == 0) {
        return DELETE;
    }
    if (strcmp(str, "save") == 0) {
        return SAVE;
    }
    if (strcmp(str, "quit") == 0) {
        return QUIT;
    }
    return UNKNOWN;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }

    Node* root = NULL;
    char* filename = argv[1];
    int count = loadAirports(filename, &root);
    printf("Загружено %d аэропортов. Система готова к работе.\n", count);

    char line[MAX_LINE];
    while (printf("\n> ") && fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n\r")] = 0;
        if (strlen(line) == 0)
            continue;

        char cmd_str[10];
        sscanf(line, "%s", cmd_str);
        Command cmd = getComand(cmd_str);

        char* arg = strchr(line, ' ');
        if (arg)
            arg++;

        switch (cmd) {
        case FIND:
            if (arg) {
                Node* res = find(root, arg);
                if (res)
                    printf("%s → %s\n", res->iata, res->name);
                else
                    printf("Аэропорт с кодом '%s' не найден в базе.\n", arg);
            }
            break;

        case ADD:
            if (arg) {
                char* colon = strchr(arg, ':');
                if (colon) {
                    *colon = '\0';
                    root = add(root, arg, colon + 1);
                    printf("Аэропорт '%s' добавлен в базу.\n", arg);
                }
            }
            break;

        case DELETE:
            if (arg) {
                if (find(root, arg)) {
                    root = deleteNode(root, arg);
                    printf("Аэропорт '%s' удалён из базы.\n", arg);
                } else {
                    printf("Аэропорт с кодом '%s' не найден.\n", arg);
                }
            }
            break;

        case SAVE:
            saveTree(root, filename);
            printf("База сохранена: %d аэропортов.\n", countNodes(root));
            break;

        case QUIT:
            freeTree(root);
            printf("Работа завершена.\n");
            return 0;

        default:
            printf("Неизвестная команда.\n");
        }
    }
    return 0;
}