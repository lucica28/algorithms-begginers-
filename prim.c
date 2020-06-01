#include <limits.h> 
#include <stdbool.h> 
#include <stdio.h> 
#include<stdlib.h>

typedef struct graph{
    int **mat;
    int nodes;
}graph; 

graph *createGraph(int N)
{
    int i;
    graph *new = malloc(sizeof(graph));
    new->nodes = N;
    
    new->mat = malloc((N + 1) * sizeof(int*));
    for(i = 1; i <= N; i++)
        new->mat[i] = calloc(N + 1, sizeof(int));

    return new;
}

void addEdge(graph *g, int v1, int v2, int cost)
{
    g->mat[v1][v2] = g->mat[v2][v1] = cost;
}

void destroyGraph(graph *g)
{
    int i;
    for(i = 1; i <= g->nodes; i++)
        free(g->mat[i]);\

    free(g->mat);
    free(g);
}

int minKey(int N, int key[], int visited[]) 
{ 
    int i;
    int min = INT_MAX, minIndex; 
  
    for (i = 1; i <= N; i++) 
        if (visited[i] == 0 && key[i] < min) 
            min = key[i], minIndex = i; 
  
    return minIndex; 
} 
  
void primMST(graph *g, FILE *fout) 
{ 
    int i, j, s = 0; 
    int key[g->nodes], visited[g->nodes], parent[g->nodes];  
    
    for (i = 1; i <= g->nodes; i++) 
        key[i] = INT_MAX, visited[i] = 0; 
   
    key[1] = 0; 
    parent[1] = -1;
   
    int count;
    for (count = 0; count < g->nodes; count++) { 
        int u = minKey(g->nodes, key, visited); 
 
        visited[u] = 1;
        s = s + key[u];
  
        for (j = 1; j <= g->nodes; j++) 
              if (g->mat[u][j] && visited[j] == 0 && g->mat[u][j] < key[j]) 
                parent[j] = u, key[j] = g->mat[u][j]; 
    } 
  
    fprintf(fout, "%d\n", s);
    for(i = 1; i <= g->nodes; i++)
        fprintf(fout, "%d ", parent[i] == -1 ? 0 : parent[i]); 
} 
   
int main() 
{ 
    FILE *fin = fopen("prim.in", "r");
    FILE *fout = fopen("prim.out", "w");
    int N, M, i, v1, v2, cost;

    fscanf(fin, "%d%d", &N, &M); 
    graph *g = createGraph(N);
    for(i = 1; i <= M; i++)
    {
        fscanf(fin, "%d%d%d", &v1, &v2, &cost);
        addEdge(g, v1, v2, cost);
    }

    primMST(g, fout);

    destroyGraph(g);
    fclose(fin);
    fclose(fout); 
  
    return 0; 
} 
