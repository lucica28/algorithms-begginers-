#include <stdio.h>
#include <stdlib.h>

typedef struct listNode{
	int node;
	struct listNode *next;
}listNode;

typedef struct graph{
	int nodes;
	listNode **adj_heads;
}graph;

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

graph *createGraph(int N)
{
	graph *g = (graph*) malloc(sizeof(graph));

    g->nodes = N;
    g->adj_heads = (listNode**) calloc(N + 1, sizeof(listNode*));

    return g;
}

void addEdge(graph *g, int v1, int v2)
{
	listNode *new;
    new = (listNode *) malloc(sizeof(listNode));
    
    new->node = v2;
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
        {
        	freeList(g->adj_heads[i]);
        }

    free(g->adj_heads);
    free(g);
}

void BFS(graph *g, int source, FILE *fout)
{
	int *d = malloc((g->nodes + 1) * sizeof(int));
	int *visited = calloc(g->nodes + 1, sizeof(int));
	
	for(int i = 1; i <= g->nodes; i++)
		if(i == source) d[i] = 0;
			else d[i] = -1;

	queue *q;
	initQueue(&q);

	enqueue(q, source);
	visited[source] = 1;

	while(!isEmptyQueue(q))
	{
		int aux = front(q);
		
		listNode *it = g->adj_heads[aux];
		while(it)
		{
			if(!visited[it->node]) 
			{
				d[it->node] = d[aux] + 1;
				enqueue(q, it->node);
				visited[it->node] = 1;
			}
			it = it->next;
		}

		dequeue(q);
	}

	for(int i = 1; i <= g->nodes; i++)
		fprintf(fout, "%d ", d[i]);
	fprintf(fout, "\n");

	free(d);
	free(visited);
	free(q);
}

int main()
{
	FILE *fin = fopen("bfs.in", "r");
	FILE *fout = fopen("bfs.out", "w");
	int N, M, source, v1, v2;

	fscanf(fin, "%d%d%d", &N, &M, &source);
	graph *g = createGraph(N);

	for(int i = 1; i <= M; i++)
	{
		fscanf(fin, "%d%d", &v1, &v2);
		addEdge(g, v1, v2);
	}

	BFS(g, source, fout);
	destroyGraph(g);

	fclose(fin);
	fclose(fout);
}