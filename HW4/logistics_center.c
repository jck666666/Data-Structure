#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define INFINITE 99999999
#define MaxVertex 99999

FILE *in, *out;
int N, M;  // N 可能需要出貨的地點   M 有訂單需求的地點數量

int edge[MaxVertex][MaxVertex];
int selected[MaxVertex];
int dis[MaxVertex];

typedef struct graphnode *nptr;
typedef struct graphnode {
    int v;
    int edge;
    nptr next;
} graphnode;
graphnode graph[MaxVertex];

typedef struct Order {
    int s;  // s 請求訂單的地點
    int t;  //t 請求的訂單數

} Order;
Order O[MaxVertex];

/* --------------- Queue ---------------- */
int isinQ[MaxVertex];
int Q[MaxVertex];
int f = 0, r = -1;

void enQ(int t) {
    if (r == MaxVertex - 1) {  //full
        return;
    }
    r++;
    Q[r] = t;
    isinQ[t] = TRUE;
}

int deQ() {
    if (r < f) {  //empty
        return -1;
    }

    int ver = Q[f];
    f++;

    return ver;
}

void printQ() {
    printf("[ ");
    for (int i = f; i <= r; i++) {
        printf("%d ", Q[i]);
    }
    printf("]\nf:%d r:%d\n", f, r);
}

/* --------------- Function ---------------- */

void initEdge(int N) {
    for (int i = 0; i <= N; i++) {
        edge[i][0] = -1;
    }
    for (int j = 0; j <= N; j++) {
        edge[0][j] = -1;
    }

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            edge[i][j] = INFINITE;
        }
    }
}

void printAdjList(int N) {
    printf("\nAdjacency list : \n");

    nptr ptr;
    for (int i = 1; i <= N; i++) {
        ptr = graph[i].next;
        printf("(%d) → ", graph[i].v);

        while (ptr != NULL) {
            printf("%d → ", ptr->v);
            ptr = ptr->next;
        }
        printf("NULL\n");
    }
    printf("\n");
}

void makeAdjList(int N) {
    nptr successor, new;

    for (int i = 1; i <= N; i++)  //row
    {
        graph[i].v = i;  //graph[0],graph[1],graph[2],....,graph[n-1]
        successor = NULL;

        for (int j = N; j >= 1; j--)  //col
        {
            if (edge[i][j] != INFINITE) {
                new = (nptr)malloc(sizeof(graphnode));
                new->v = j;
                new->edge = edge[i][j];
                new->next = successor;
                successor = new;
            }
        }

        graph[i].next = successor;  //graph[0],graph[1],graph[2],....,graph[n-1]
    }
}

/* --------------- Function ---------------- */
void init() {
    f = 0, r = -1;
    for (int x = 0; x <= N; x++)  //initilize dis[x]
    {
        selected[x] = FALSE;
        dis[x] = INFINITE;
        isinQ[x] = FALSE;
        Q[x] = 0;
    }
}

void PrintDis(int N) {
    for (int vertex = 1; vertex <= N; vertex++) {
        printf("%d ", dis[vertex]);
    }
    printf("\n");
}

void bfs(int start) {
    enQ(start);
    dis[start] = 0;

    while (r >= f) {  // not empty
        int ver = deQ();
        selected[ver] = TRUE;

        for (nptr ptr = graph[ver].next; ptr != NULL; ptr = ptr->next) {
            if (dis[ver] + ptr->edge < dis[ptr->v]) {
                dis[ptr->v] = dis[ver] + ptr->edge;
            }

            if (selected[ptr->v] == FALSE && isinQ[ptr->v] == FALSE) {
                enQ(ptr->v);
            }
        }
    }
}

/* --------------- Function ---------------- */

int main() {
    in = fopen("input_2.txt", "r");
    out = fopen("myoutput2.txt", "w");

    if (in == NULL || out == NULL) {
        perror("Error ");
        return -1;
    }

    fscanf(in, "%d %d ", &N, &M);
    //printf("%d %d\n", N, M);

    initEdge(N);
    int u, v, p;  // uv 路徑兩端的地點   p 路徑長
    for (int i = 1; i < N; i++)  //N-1條邊
    {
        fscanf(in, "%d %d %d ", &u, &v, &p);
        //printf("%d %d %d\n", u, v, p);

        edge[u][v] = edge[v][u] = p;  //add edge
        //printEdge(size);
    }
    makeAdjList(N);
    //printAdjList(N);

    for (int i = 1; i <= M; i++) {
        fscanf(in, "%d %d ", &O[i].s, &O[i].t);
        // printf("%d %d\n", O[i].s, O[i].t);
    }

    int minCost = INT_MAX;
    int flag = 0;
    int pos[MaxVertex];

    for (int start = 1; start <= N; start++) {
        init();

        // printf("\nstart: %d\n", start);
        bfs(start);
        //PrintDis(N);

        int cost = 0;
        for (int i = 1; i <= M; i++) {
            cost += dis[O[i].s] * O[i].t;
        }

        if (cost < minCost) {
            minCost = cost;
            flag = 0;
            pos[flag++] = start;
        }
        else if (cost == minCost) {
            pos[flag++] = start;
        }
    }
    minCost *= 2;  //depart and back
    printf("\n%d\n", minCost);
    fprintf(out, "%d\n", minCost);

    for (int i = 0; i < flag; i++) {
        printf("%d ", pos[i]);
        fprintf(out, "%d ", pos[i]);
    }
}
