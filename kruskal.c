#include <stdio.h>
#include <stdlib.h>

typedef struct listNode{
    // Costul dintre detinatorul listei si node
    int node;
    struct listNode *next;
} listNode;

typedef struct graph{
    // Numarul de noduri ale grafului
    int nodes;
    // adj_heads[i] -> lista de adiacenta a nodului i 
    listNode **adj_heads;
} graph;

typedef struct edgesList{
	int src, dest, cost;
} edgesList;

graph *createGraph(int nodes)
{
    graph *g = (graph*) malloc(sizeof(graph));

    g->nodes = nodes;
    g->adj_heads = (listNode**) calloc(nodes + 1, sizeof(listNode*));

    return g;
}

void addEdge(graph *g, int v1, int v2)
{
    listNode *new;
    new = (listNode *) malloc(sizeof(listNode));
    new->node = v2;
    new->next = g->adj_heads[v1];
    g->adj_heads[v1] = new;

    listNode *new2 = malloc(sizeof(listNode));
    new2->node = v1;
    new2->next = g->adj_heads[v2];
    g->adj_heads[v2] = new2;
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
	int i;
    for (i = 0; i <= g->nodes; i++)
        freeList(g->adj_heads[i]);

    free(g->adj_heads);
    free(g);
}

void dfs(graph *g, int source, int *visited)
{
	visited[source] = 1;

	listNode *it = g->adj_heads[source];
	while(it)
	{
		if(!visited[it->node]) dfs(g, it->node, visited);
		it = it->next;
	}
}

int formCycle(graph *g, int v1, int v2)
{
	int *visited = calloc(g->nodes + 1, sizeof(int));
	dfs(g, v1, visited);
	if(visited[v2]) {free(visited); return 1;}
	free(visited);	
	return 0;
}

void swapInt(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

void swap(edgesList *e1, edgesList *e2)
{
	swapInt(&e1->src, &e2->src);
	swapInt(&e1->dest, &e2->dest);
	swapInt(&e1->cost, &e2->cost);
}

int main()
{
	FILE *fin = fopen("kruskal.in", "r");
	FILE *fout = fopen("kruskal.out", "w");

	int N, M, i, j;
	edgesList *edges, *ans;

	fscanf(fin, "%d%d", &N, &M);
	edges = calloc(M+ 1, sizeof(edgesList));
	ans = calloc(N, sizeof(edgesList));

	for(i = 1; i <= M; i++)
		fscanf(fin, "%d%d%d", &edges[i].src, &edges[i].dest, &edges[i].cost);
	
	for(i = 1; i < M; i++)
		for(j = i + 1; j <= M; j++)
			if(edges[i].cost > edges[j].cost) swap(&edges[i], &edges[j]);

	graph *g = createGraph(N);
	int it = 1, sum = 0;

	for(j = 1; j <= M; j++)
	{
		if(!formCycle(g, edges[j].src, edges[j].dest))
		{
			addEdge(g, edges[j].src, edges[j].dest);
			ans[it].src = edges[j].src;
			ans[it].dest = edges[j].dest;
			sum = sum + edges[j].cost;
			it++;
		}
		if(it == N) break;
	}

	fprintf(fout, "%d\n", sum);
	for(i = 1; i <= N - 1; i++)
		fprintf(fout, "%d %d\n", ans[i].src, ans[i].dest);

	free(edges);
	free(ans);
	destroyGraph(g);
	fclose(fin);
	fclose(fout);

	return 0;
}