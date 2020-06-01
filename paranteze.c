#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct Node
{
    char value;
    int pos;
    struct Node *next;
} Node;

typedef struct Stack
{
    Node *head;

} Stack;

void initStack(Stack **s)
{
    *s = (Stack *) malloc(sizeof(Stack));

    (*s)->head = NULL;
}

int isEmptyStack(Stack *s)
{
    return s->head == NULL ? 1 : 0;
}

void push(Stack *s, char c, int position)
{
    Node *new = (Node *) malloc(sizeof(Node));
    new->value = c;
    new->pos = position;
    
    new->next = s->head;
    s->head = new;
}

void pop(Stack *s)
{
    if (isEmptyStack(s) == 1)
        return;

    Node *delete = s->head;
    s->head = s->head->next;
    free(delete);
}

char topChar(Stack *s)
{
	if(isEmptyStack(s)) return 'x';
    return s->head->value;
}

int topInt(Stack *s)
{
	if(isEmptyStack(s)) return -1;
    return s->head->pos;
}

int main()
{
    FILE *fin = fopen("paranteze.in", "r");
    FILE *fout = fopen("paranteze.out", "w");
    Stack *s;
    int N, max = 0;
    char str[1000000];

    fscanf(fin, "%d", &N);
    initStack(&s);
    fscanf(fin, "%s", str);

    for(int i = 0; i < strlen(str); i++)
    {
        if(str[i] == '(') push(s, '(', i);
        if(str[i] == '[') push(s, '[', i);
        if(str[i] == '{') push(s, '{', i);
        
        if(str[i] == ')') 
        {
            if(topChar(s) == '(')
            {
                if(max < i - topInt(s) + 1) max = i - topInt(s) + 1;
                pop(s);
            }
            else push(s, ')', i);
        }
        
        if(str[i] == ']') 
        {
            if(topChar(s) == '[')
            {
                if(max < i - topInt(s) + 1) max = i - topInt(s) + 1;
                pop(s);
            }
            else push(s, ']', i);
        }
        
        if(str[i] == '}') 
        {
            if(topChar(s) == '{')
            {
                if(max < i - topInt(s) + 1) max = i - topInt(s) + 1;
                pop(s);
            }
            else push(s, '}', i);
        }
    }

    fprintf(fout, "%d", max);
    
    while(!isEmptyStack(s)) 
    {
		pop(s);
    }
    free(s);

    fclose(fin);
    fclose(fout);
}
