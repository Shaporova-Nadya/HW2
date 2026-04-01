#pragma once

typedef struct Edge {
    int num;
    int len;
    struct Edge* next;
} Edge;

typedef struct City {
    struct Edge* head;
} City;

typedef struct HeapNode {
    int dist;
    int cityNum;
} HeapNode;

typedef struct MinHeap {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

/* Добавляет новое ребро в список смежности */
void addEdge(City* graph, int u, int v, int len);

/* Освобождает память, выделенную под граф */
void freeGraph(City* graph, int n);

/* Инициализация бинарной кучи */
MinHeap* initHeap(int maxSize);

/* Добавляет новый элемент в кучу */
void push(MinHeap* h, int dist, int cityNum);

/* Извлекает элемент с минимальным расстоянием из кучи */
HeapNode popMin(MinHeap* h);
