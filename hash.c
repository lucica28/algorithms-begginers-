#include <stdio.h>
#include <stdlib.h>
#define MOD 666013

typedef struct node{
	int val;
	struct node *next;
}node;

typedef struct list{
	node *head;
}list;

typedef struct hashTable{
	list **arr;
	int size;
} hashTable;

list *createList()
{
	list *new = malloc(sizeof(list));
	new->head = NULL;
	
	return new;
}

void insert(list *l, int x)
{
	//insert as first element
	node *new = malloc(sizeof(node));
	new->val = x;
	new->next = l->head;
	l->head = new;
}

void del(list *l, int x)
{
	//empty list
	if(!l->head) return;

	//first element
	if(l->head->val == x)
	{
		node *del = l->head;
		l->head = l->head->next;
		free(del);
		return;
	}

	node *cur = l->head, *prev;
	while(cur)
	{
		if(cur->val == x) break;
		prev = cur;
		cur = cur->next;	
	}

	//other element
	if(cur)
	{
		prev->next = cur->next;
		free(cur);
	}
}

int isIn(list *l, int x)
{
	node *it = l->head;
	while(it)
	{
		if(it->val == x) return 1;
		it = it->next;
	}

	return 0;
}

void delList(list *l)
{
	node *cur = l->head, *prev;
	while(cur)
	{
		prev = cur;
		cur = cur->next;
		free(prev);
	}
}

hashTable *createHashTable(int size)
{
	hashTable *new = malloc(sizeof(hashTable));
	new->size = size;
	
	new->arr = malloc(size * sizeof(list*));
	for(int i = 0; i < size; i++)
		new->arr[i] = createList();

	return new;
}

void delHashTable(hashTable *h)
{
	for(int i = 0; i < h->size; i++)
	{
		delList(h->arr[i]);
		free(h->arr[i]);
	}
	free(h->arr);
	free(h);
}

int main()
{
	FILE *fin = fopen("hashuri.in", "r");
	FILE *fout = fopen("hashuri.out", "w");
	int nOperations, operation, x, hashVal;
	hashTable *h = createHashTable(MOD);

	fscanf(fin, "%d", &nOperations);

	for(int i = 1; i <= nOperations; i++)
	{
		fscanf(fin, "%d%d", &operation, &x);
		hashVal = x % MOD;

		switch(operation)
		{
			case 1:
				if(!isIn(h->arr[hashVal], x)) insert(h->arr[hashVal], x);
				break;

			case 2:
				del(h->arr[hashVal], x);
				break;

			case 3:
				fprintf(fout, "%d\n", isIn(h->arr[hashVal], x));
				break;
		}
	}

	delHashTable(h);
	fclose(fin);
	fclose(fout);
}