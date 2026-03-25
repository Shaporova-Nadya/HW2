#pragma once
#include <stdio.h>

typedef struct Node{
    char iata[5]; //Key for finding the whole name
    char* name;//Name of the airport
    struct Node* left;//left child
    struct Node* right;//right child
    int height;//in AVL we keeping height for balancing
}Node;

void updateHeight(Node* n);//this function updates height

Node* rotateLeft(Node* a);//left rotation

Node* rotateRight(Node* a);//right rotation

Node* bigRotateRight(Node* a);//big right rotation

Node* bigRotateLeft(Node* a);//big left rotation

Node* find(Node* root, const char* iata);//function for finding airport, using iata

int getDifference(Node* n);//function that gets high difference

Node* balance(Node* p);//function that makes tree balanced

Node* add(Node* root, const char* code, const char* name);//function for adding new airport code with name

int loadAirports(const char* filename, Node** root);//function reads .txt file; divide strings by ':'

void freeTree(Node* root);//free memory which was used by tree

void saveNode(Node* root, FILE* f);//writes node's data into file

void saveTree(Node* root, const char* filename);//writes into file

Node* deleteNode(Node* root, const char* code);//deletes node, using airport code

Node* findMin(Node* p);//finds min element

int countNodes(Node* root);//counts number of airports