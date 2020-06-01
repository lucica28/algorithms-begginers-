#include <stdio.h>
#include <stdlib.h>
#define inf 9999999

typedef struct listNode{
    // Nodul cu care are muchie detinatorul listei 
    int node;
    // Costul dintre detinatorul listei si node
    int cost;
    struct listNode *next;
} listNode;

typedef struct graph{
    // Numarul de noduri ale grafului
    int nodes;
    // adj_heads[i] -> lista de adiacenta a nodului i 
    listNode **adj_heads;
} graph;

typedef struct heap{
    int *arr;
    int size;
}heap;

heap *createHeap(int n)
{
    heap *new = malloc(sizeof(heap));
    new->arr = calloc(n + 1, sizeof(int));
    new->size = 0;

    return new;
}

int getLeftChild(int index)
{
    return 2 * index;
}

int getRightChild(int index)
{
    return 2 * index + 1;
}

int getParent(int index)
{
    return index / 2;
}

void swap(heap *h, int i, int j)
{
    int temp = h->arr[i];
    h->arr[i] = h->arr[j];
    h->arr[j] = temp;
}

int hasLeftChild(heap *h, int index)
{
    return getLeftChild(index) <= h->size;
}

int hasRightChild(heap *h, int index)
{
    return getRightChild(index) <= h->size;
}

void heapifyUp(heap *h, int index, int d[], int poz[])
{
    while(index > 1)
    {
        int parent = getParent(index);

        if(d[h->arr[parent]] > d[h->arr[index]])
        {
            poz[h->arr[index]] = parent;
            poz[h->arr[parent]] = index;
            swap(h, parent, index);
            index = parent;
        }
        else index = 1;
    }
}

void heapifyDown(heap *h, int index, int d[], int poz[])
{
    int f;

    while(index <= h->size)
    {
        f = index;

        if(hasLeftChild(h, index))
        {
            if(hasRightChild(h, index) && 
                d[h->arr[getRightChild(index)]] < d[h->arr[getLeftChild(index)]])
                    f = getRightChild(index);
            else f = getLeftChild(index);
        }
        else return;

        if(d[h->arr[index]] > d[h->arr[f]]){
            poz[h->arr[index]] = f;
            poz[h->arr[f]] = index;
            swap(h, index, f);
            index = f;
        }
        else return;
    }
}

void addEdge(graph *g, int v1, int v2, int cost)
{
    listNode *new;
    new = (listNode *) malloc(sizeof(listNode));
    new->node = v2;
    new->cost = cost;
    new->next = g->adj_heads[v1];
    g->adj_heads[v1] = new;
}

void freeList(listNode *list) {
    listNode *it = list, *prev;

    while (it != NULL) {
        prev = it;
        it = it->next;
        free(prev);
    }
}

void destroyGraph(graph *g)
{   
    for (int i = 0; i <= g->nodes; i++)
        freeList(g->adj_heads[i]);

    free(g->adj_heads);
    free(g);
}


graph *createGraph(int nodes)
{
    graph *g = (graph*) malloc(sizeof(graph));

    g->nodes = nodes;
    g->adj_heads = (listNode**) calloc(nodes + 1, sizeof(listNode*));

    return g;
}

void dijkstra(graph *g, heap *h, int source, int d[], int poz[])
{
    int *pred = calloc(g->nodes, sizeof(int));
    
    for ( int i = 1; i <= g->nodes; ++i )
        if(i != source) d[i] = inf, poz[i] = -1, pred[i] = 1;
    
    poz[source] = 1;
    h->arr[++h->size] = source;
    
    while(h->size)
    {
        int min = h->arr[1];
        swap(h, 1, h->size);
        poz[h->arr[1]] = 1;
        h->size--;

        heapifyDown(h, 1, d ,poz);

        listNode *it = g->adj_heads[min];
        while (it)
        {
            if (d[it->node] > d[min] + it->cost)
            {
                d[it->node] = d[min] + it->cost;
                pred[it->node] = min;
                if (poz[it->node] != -1)
                    heapifyUp(h, poz[it->node], d, poz); 
                else
                {
                    h->arr[++h->size] = it->node;
                    poz[h->arr[h->size]] = h->size;
                    heapifyUp(h, h->size, d, poz);
                }
            }
            it = it->next;
        }    
    }
    for(int i = 2; i <= g->nodes; i++)
        printf("%d ", pred[i]);
}

int main()
{
    graph *g;
    heap *h;
    FILE *fin = fopen("dijkstra.in", "r");
    FILE *fout = fopen("dijkstra.out", "w");
    int N, M, v1, v2, cost, d[100001], q[100001];
    
    fscanf(fin, "%d%d", &N, &M);
    g = createGraph(N);
    h = createHeap(N);
    
    for(int i = 1; i <= N; i++)
        d[i] = q[i] = 0;
    
    for(int i = 1; i <= M; i++)
    {
        fscanf(fin, "%d%d%d", &v1, &v2, &cost);
        addEdge(g, v1, v2, cost);
    }
    
   dijkstra(g, h, 1, d, q);

   for(int i = 1; i <= N; i++)
        fprintf(fout, "%d ", d[i] == inf ? -1 : d[i]);
    fprintf(fout, "\n");

    destroyGraph(g);
    free(h->arr);
    free(h);
    fclose(fin);
    fclose(fout);
}