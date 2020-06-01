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

typedef struct node{
	int val;
	struct node *next;
}node;

typedef struct queue
{
    node *front;
    node *rear;
}queue;

void initQueue(queue **q)
{
    *q = (queue *) malloc(sizeof(queue));
    (*q)->front = (*q)->rear = NULL;
}

int isEmptyQueue(queue *q)
{
    return q->front == NULL ? 1 : 0;
}

void enqueue(queue *q, int x)
{
    node *new = malloc(sizeof(node));
    new->val = x;
    new->next = NULL;

    if (isEmptyQueue(q)) {
        q->front = q->rear = new;
        return;
    }

    q->rear->next = new;
    q->rear = new;
}

void dequeue(queue *q)
{
    if (isEmptyQueue(q) == 1)
        return;

    node *del = q->front;
    q->front = q->front->next;
    free(del);

    if (q->front == NULL)
        q->rear = NULL;
}

int front(queue *q)
{
    return q->front->val;
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

void Bellman_Ford(graph *g, FILE *fout){
	int *timesInQueue = calloc(g->nodes + 1, sizeof(int));
	int *isInQueue = calloc(g->nodes + 1, sizeof(int));
	int negativeCycle = 0;
	int *d = calloc(g->nodes + 1, sizeof(int));
	queue *q;

	initQueue(&q);
	isInQueue[1] = 1;
	timesInQueue[1]++;
	enqueue(q, 1);

	for(int i = 2; i <= g->nodes; i++)
		d[i] = inf;

	while(!isEmptyQueue(q)){
		int aux = front(q);
		dequeue(q);
		isInQueue[aux] = 0;

		listNode *it = g->adj_heads[aux];
		while(it){
			if(d[it->node] > d[aux] + it->cost){
				d[it->node] = d[aux] + it->cost;

				if(!isInQueue[it->node]){
					if(timesInQueue[it->node] > g->nodes) negativeCycle = 1;
				
					else{
						enqueue(q, it->node);
						isInQueue[it->node] = 1;
						timesInQueue[it->node]++;
					}
				}
			}
			it = it->next;
		}
	}

	if(negativeCycle) fprintf(fout, "Ciclu negativ!");
	else{
		for(int i = 2; i <= g->nodes; i++)
			fprintf(fout, "%d ", d[i]);
	}
	fprintf(fout, "\n");

	free(d);
	free(isInQueue);
	free(timesInQueue);
	free(q);
}

int main()
{
    graph *g;
    FILE *fin = fopen("bellmanford.in", "r");
    FILE *fout = fopen("bellmanford.out", "w");
    int N, M, v1, v2, cost;
    
    fscanf(fin, "%d%d", &N, &M);
    g = createGraph(N);
    
    for(int i = 1; i <= M; i++)
    {
        fscanf(fin, "%d%d%d", &v1, &v2, &cost);
        addEdge(g, v1, v2, cost);
    }
    
   Bellman_Ford(g, fout);

    destroyGraph(g);
    fclose(fin);
    fclose(fout);
}