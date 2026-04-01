#pragma once

typedef struct Edge {
    int num; //Номер города, куда ведет дорога
    int len; //длина пути
    struct Edge* next; //указатель на следующее ребро этого города
} Edge;

typedef struct City {
    struct Edge* head; //Указатель на первое ребро в списке смежности
} City;

typedef struct HeapNode {
    int dist; //Расстояние
    int cityNum; //Номер города
} HeapNode;

typedef struct MinHeap {
    HeapNode* data; //Массив, где лежат элементы кучи
    int size; //Текущее количество элементов
    int capacity; //Максимальный размер
} MinHeap;

void addEdge(City* graph, int u, int v, int len); //Добавляет новое ребро в список смежности

void freeGraph(City* graph, int n); //Освобождает память, выделенную под граф

MinHeap* initHeap(int maxSize); //Инициализация бинарной кучи

void push(MinHeap* h, int dist, int cityNum); //Добавляет новый элемент в кучу

HeapNode popMin(MinHeap* h); //Извлекает элемент с минимальным расстоянием из кучи
