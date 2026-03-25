#include "avl.h"
#include <stdio.h>
#include<stdlib.h>
#include <string.h>

int getHeight(Node* n)
{
    return n ? n->height : 0;
}

void updateHeight(Node* n)
{
    int hl = getHeight(n->left);
    int hr = getHeight(n->right);
    n->height = (hl > hr ? hl : hr) + 1;
}

Node* rotateLeft(Node* a)
{
    Node* b = a->right;
    Node* c = b->left;
    b->left = a;
    a->right = c;

    updateHeight(a);
    updateHeight(b);
    return b;
}

Node* rotateRight(Node* a)
{
    Node* b = a->left;
    Node* c = b->right;
    b->right = a;
    a->left = c;

    updateHeight(a);
    updateHeight(b);
    return b;
}

Node* bigRotateRight(Node* a)
{
    a->left = rotateLeft(a->left);
    return rotateRight(a);
}

Node* bigRotateLeft(Node* a)
{
    a->right = rotateRight(a->right);
    return rotateLeft(a);
}

Node* find(Node* root, const char* iata)
{
    if(root == NULL){
        return NULL;
    }
    int res = strcmp(iata, root->iata);
    if(res == 0){
        return root;
    }
    if (res < 0){
        return find(root->left, iata);
    } else {
        return find(root->right, iata);
    }
}

int getDifference(Node* n)
{
    if(n == NULL){
        return 0;
    }
    return getHeight(n->left) - getHeight(n->right);
}

Node* balance(Node* p)
{
    if(p == NULL){
        return NULL;
    }
    updateHeight(p);
    
    int dif = getDifference(p);

    if(dif == 2){
        if(p->left && getDifference(p->left) < 0){
            return bigRotateRight(p);
     }
        return rotateRight(p);
    }

    if(dif == -2){
        if(p->right && getDifference(p->right) > 0){
            return bigRotateLeft(p);
        }
        return rotateLeft(p);
    }

    return p;
}

Node* add(Node* root, const char* code, const char* name)
{
    if(root == NULL){
        Node* newNode = (Node*)malloc(sizeof(Node));

        strncpy(newNode->iata, code, 4);
        newNode->iata[4] = '\0';

        newNode->name = strdup(name);
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        
        return newNode;
    }

    int cmp = strcmp(code, root->iata);

    if(cmp < 0){
        root->left = add(root->left, code, name);
    } else if(cmp > 0) {
        root->right = add(root->right, code, name);
    } else {
        free(root->name);
        root->name = strdup(name);

        return root;
    }

    return balance(root);
}

int loadAirports(const char* filename, Node** root)
{
    FILE* f = fopen(filename, "r");
    if(!f){
        printf("Не удалось открыть файл\n");
        return -1;
    }

    char line[512];
    int count = 0;

    while(fgets(line, sizeof(line), f)){
        line[strcspn(line, "\n\r")] = 0;
        char* colon = strchr(line, ':');
        if(colon){
            *colon = '\0';
            char* code = line;
            char* name = colon + 1;
            *root = add(*root, code, name);
            count++;
        }
    }
    fclose(f);

    return count;
}

void freeTree(Node* root)
{
    if(root == NULL){
        return;
    }

    freeTree(root->left);
    freeTree(root->right);

    free(root->name);
    free(root);
}

void saveNode(Node* root, FILE* f)
{
    if(root == NULL){
        return;
    }

    saveNode(root->left, f);
    fprintf(f, "%s: %s\n", root->iata, root->name);
    saveNode(root->right, f);
}

void saveTree(Node* root, const char* filename)
{
    FILE* f = fopen(filename, "w");
    if(!f){
        printf("Ошибка открытия файла");
        return;
    }
    saveNode(root, f);
    fclose(f);
}

Node* findMin(Node* p)
{
    return p->left ? findMin(p->left) : p;
}

Node* deleteNode(Node* root, const char* code)
{
    if(root == NULL){
        return NULL;
    }

    int cmp = strcmp(code, root->iata);
    if(cmp < 0){
        root->left = deleteNode(root->left, code);
    } else if(cmp > 0) {
        root->right = deleteNode(root->right, code);
    } else {

        Node* l = root->left;
        Node* r = root->right;
        free(root->name);
        free(root);

        if(!r){
            return l;
        }

        Node* min = findMin(r);
        Node* newNode = (Node*)malloc(sizeof(Node));
        strncpy(newNode->iata, min->iata, 5);
        newNode->name = strdup(min->name);

        newNode->right = deleteNode(r, min->iata);
        newNode->left = l;
    }
    return balance(root);
}

int countNodes(Node* root) 
{
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}
