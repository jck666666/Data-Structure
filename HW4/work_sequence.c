#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MaxJob 2000

FILE *in, *out;

typedef struct node *nptr;
typedef struct node
{
    int vertex;
    nptr next;
} graphnode;

typedef struct headnode
{
    int indegree;
    int pre;
    nptr next;
} headnode;
headnode graph[MaxJob];

int queue[MaxJob];
int f = 0, r = -1;

/* --------------- Function ---------------- */

void initEdge(int size, int edge[][size])
{
    for (int i = 0; i < size; i++)
    {
        edge[i][0] = -1;
    }
    for (int j = 0; j < size; j++)
    {
        edge[0][j] = -1;
    }

    for (int i = 1; i < size; i++)
    {
        for (int j = 1; j < size; j++)
        {
            edge[i][j] = 0;
        }
    }
}

void printEdge(int size, int edge[][size])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d ", edge[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printAOV(int size, nptr ptr)
{
    for (int i = 1; i < size; i++)
    {
        ptr = graph[i].next;
        printf("(%d) → ", i);

        while (ptr != NULL)
        {
            printf("%d → ", ptr->vertex);
            ptr = ptr->next;
        }
        printf("NULL\n");
    }
}

void makeAOV(int size, int edge[][size])
{
    nptr successor, ptr;

    for (int i = 1; i < size; i++)
    {
        successor = NULL;

        for (int j = size - 1; j >= 1; j--)
        {
            if (edge[i][j] == 1)
            {
                ptr = (nptr)malloc(sizeof(graphnode));
                ptr->vertex = j;
                ptr->next = successor;
                successor = ptr;

                graph[j].indegree++;
            }
        }
        graph[i].next = successor;
    }

    printAOV(size, ptr);
}

void swap(int *a, int *b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
void enQ(int v)
{
    if (r == MaxJob - 1)
    {
        printf("Q is FULL\n");
        return;
    }
    queue[++r] = v;

    int i = r;

    while ((i - 1 >= f) && (queue[i] < queue[i - 1]))
    {
        swap(&queue[i], &queue[i - 1]);
        i--;
    }
}

int deQ()
{
    if (r < f)
    {
        printf("Q is EMPTY\n");
        return 0;
    }
    return queue[f++];
}

void pQ()
{
    for (int i = f; i <= r; i++)
    {
        printf("%d ", queue[i]);
    }
}
void topological_sort(int J)
{
    int top = 0;
    nptr ptr;

    for (int i = 1; i <= J; i++)
    {
        if (graph[i].indegree == 0)
        {
            enQ(i);

            //printf("%d", i);
            //printf(" -%d %d-\n", f, r);
        }
    }

    for (int i = 1; i <= J; i++)
    {
        int v = deQ();
        printf("%d ", v);
        fprintf(out, "%d ", v);

        ptr = graph[v].next;

        while (ptr != NULL)
        {
            int xx = ptr->vertex;
            graph[xx].indegree--;

            if (graph[xx].indegree == 0)
            {
                enQ(xx);
                //  printf("%d", xx);
                //  printf(" -%d %d-\n", f, r);
            }
            ptr = ptr->next;
        }
    }
}

/* --------------- Function ---------------- */

int main()
{
    in = fopen("input_1.txt", "r");
    out = fopen("myoutput_1.txt", "w");

    if (in == NULL || out == NULL)
    {
        perror("Error: ");
        return -1;
    }

    int J, R; //Job , Relation
    fscanf(in, "%d %d ", &J, &R);
    int size = J + 1;

    int edge[size][size];
    initEdge(size, edge);
    //printEdge(size, edge);

    for (int r = 0; r < R; r++)
    {
        int i, j;
        fscanf(in, "%d %d ", &i, &j);
        edge[i][j] = 1; //add edge
    }
    //printEdge(size, edge);

    makeAOV(size, edge);
    topological_sort(J);

    return 0;
}