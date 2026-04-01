#include "states.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* f = fopen("input.txt", "r");
    if (f == NULL) {
        perror("Не удалось открыть файл");
        return 1;
    }
    int n, m;
    if (fscanf(f, "%d %d", &n, &m) != 2) {
        fclose(f);
        return 1;
    }
    City* graph = (City*)calloc(n + 1, sizeof(City));
    for (int i = 0; i < m; i++) {
        int u, v, len;
        fscanf(f, "%d %d %d", &u, &v, &len);
        addEdge(graph, u, v, len);
        addEdge(graph, v, u, len);
    }
    int k;
    fscanf(f, "%d", &k);

    int* owner = (int*)calloc(n + 1, sizeof(int));
    int** results = (int**)malloc((k + 1) * sizeof(int*));
    int* resCounts = (int*)calloc(k + 1, sizeof(int));

    MinHeap** heaps = (MinHeap**)malloc((k + 1) * sizeof(MinHeap*));

    for (int i = 1; i <= k; i++) {
        int capital;
        fscanf(f, "%d", &capital);
        owner[capital] = i;
        results[i] = (int*)malloc(n * sizeof(int));
        results[i][resCounts[i]++] = capital;

        heaps[i] = initHeap(m);

        Edge* e = graph[capital].head;
        while (e) {
            push(heaps[i], e->len, e->num);
            e = e->next;
        }
    }
    fclose(f);

    int distributed = k;
    while (distributed < n) {
        int added = 0;
        for (int i = 1; i <= k; i++) {
            while (heaps[i]->size > 0) {
                HeapNode best = popMin(heaps[i]);
                if (owner[best.cityNum] == 0) {
                    int newV = best.cityNum;
                    owner[newV] = i;
                    results[i][resCounts[i]++] = newV;
                    distributed++;
                    added++;

                    Edge* e = graph[newV].head;
                    while (e) {
                        if (owner[e->num] == 0) {
                            push(heaps[i], e->len, e->num);
                        }
                        e = e->next;
                    }
                    break;
                }
            }
        }
        if (added == 0) {
            break;
        }

        for (int i = 1; i <= k; i++) {
            printf("%d: ", i);
            for (int j = 0; j < resCounts[i]; j++)
                printf("%d ", results[i][j]);
            printf("\n");
        }

        freeGraph(graph, n);

        for (int i = 1; i <= k; i++) {

            free(heaps[i]->data);
            free(results[i]);
        }

        free(heaps);
        free(results);
        free(resCounts);
        free(owner);

        return 0;
    }
}
