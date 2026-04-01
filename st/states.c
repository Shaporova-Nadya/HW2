#include "states.h"
#include <stdio.h>
#include <stdlib.h>

void addEdge(City* graph, int u, int v, int len)
{
    Edge* newE = (Edge*)malloc(sizeof(Edge));
    newE->num = v;
    newE->len = len;
    newE->next = graph[u].head;
    graph[u].head = newE;
}

void freeGraph(City* graph, int n)
{
    for (int i; i <= n; i++) {
        Edge* curr = graph[i].head;
        while (curr) {
            Edge* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(graph);
}

MinHeap* initHeap(int maxSize)
{
    MinHeap* h = (MinHeap*)malloc(sizeof(MinHeap));
    h->data = (HeapNode*)malloc(sizeof(HeapNode) * (maxSize + 1));
    h->size = 0;
    h->capacity = maxSize;

    return h;
}

void push(MinHeap* h, int dist, int cityNum)
{
    h->size++;
    int i = h->size;
    while (i > 1 && h->data[i].dist > dist) {
        h->data[i] = h->data[i / 2];
        i /= 2;
    }
    h->data[i].dist = dist;
    h->data[i].cityNum = cityNum;
}

HeapNode popMin(MinHeap* h)
{
    if (h->size == 0) {
        HeapNode empty = { -1, -1 };
        return empty;
    }
    HeapNode minNode = h->data[1];
    HeapNode lastNode = h->data[h->size--];
    if (h->size == 0) {
        return minNode;
    }
    int i = 1, child;
    while (i * 2 <= h->size) {
        child = i * 2;
        if (child + 1 <= h->size && h->data[child + 1].dist < h->data[child].dist) {
            child++;
        }
        if (lastNode.dist > h->data[child].dist) {
            h->data[i] = h->data[child];
            i = child;
        } else {
            break;
        }
    }
    h->data[i] = lastNode;
    return minNode;
}