#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
FILE *in, *out;
int N, E, L;  //區塊 相連的關係數 清單中項目
int gbest = INT_MAX;

typedef struct graphnode *nptr;
typedef struct graphnode {
    int vertex;
    int area;
    nptr next;
} graphnode;
graphnode *graph;

typedef struct Paint {
    char col[2];
    int inven;  //inventory
    int price;
} Paint;
Paint *plist;  //plist[]

int *pused;
int *painted;

/*------------------------------------------*/
void initEdge(int size, int edge[][size]) {
    for (int i = 0; i < size; i++) {
        edge[i][0] = -1;
    }
    for (int j = 0; j < size; j++) {
        edge[0][j] = -1;
    }

    for (int i = 1; i < size; i++) {
        for (int j = 1; j < size; j++) {
            edge[i][j] = 0;
        }
    }
}

void printAdjList(int size) {
    printf("\nAdjacency list : \n");

    nptr ptr;
    for (int i = 0; i < size - 1; i++) {
        ptr = graph[i].next;
        printf("(%d) → ", graph[i].vertex);

        while (ptr != NULL) {
            printf("%d → ", ptr->vertex);
            ptr = ptr->next;
        }
        printf("NULL\n");
    }
    printf("\n");
}

void makeAdjList(int size, int edge[][size]) {
    nptr successor, new;

    for (int i = 1; i < size; i++)  //row
    {
        graph[i - 1].vertex = i;  //graph[0],graph[1],graph[2],....,graph[n-1]
        successor = NULL;

        for (int j = size - 1; j >= 1; j--)  //col
        {
            if (edge[i][j] == 1) {
                new = (nptr)malloc(sizeof(graphnode));
                new->vertex = j;
                new->next = successor;
                successor = new;
            }
        }

        graph[i - 1].next = successor;  //graph[0],graph[1],graph[2],....,graph[n-1]
    }

    printAdjList(size);
}
/*------------------------------------------*/

int vaild(int t) {
    int color = painted[t];
    if (pused[color] > plist[color].inven) {  //over upperbound
        return FALSE;
    }

    nptr ptr = graph[t].next;

    while (ptr) {
        if (painted[ptr->vertex-1] == painted[t]) {
    
            return FALSE;
        }
        ptr = ptr->next;
    }

    return TRUE;
}

int CntCost() {
    int sum = 0;
    for (int i = 0; i < L; i++) {
        sum += (pused[i]) * (plist[i].price);
    }
    return sum;
}

void algo(int gindex) {
    if (gindex >= N) {
        int cost = CntCost();
        if (cost < gbest) {
            gbest = cost;
        }
        return;
    }

    for (int i = 0; i < L; i++) {  //paint list
        painted[gindex] = i;  //color i paint on gindex
        pused[i] += graph[gindex].area;

        if (vaild(gindex) == TRUE) {  //check if it is vaild
            algo(gindex + 1);
        }

        pused[i] -= graph[gindex].area;
        painted[gindex] = -1;
    }
}
/*------------------------------------------*/

int main() {
    in = fopen("input_3.txt", "r");
    out = fopen("myoutput3.txt", "w");
    if (in == NULL || out == NULL) {
        perror("Error ");
        return -1;
    }

    //int N, E, L;  //區塊 相連的關係數 清單中項目
    fscanf(in, "%d %d %d ", &N, &E, &L);
    printf("%d %d %d\n", N, E, L);
    graph = malloc(N * sizeof(graphnode));
    painted = malloc(N * sizeof(graphnode));  //record which paint painted on the vertex
    plist = malloc(L * sizeof(Paint));
    pused = malloc(L * sizeof(Paint));  //record how much paint is used

    int edge[N + 1][N + 1];
    initEdge(N + 1, edge);

    for (int i = 0; i < N; i++) {
        painted[i] = -1;
        fscanf(in, "%d ", &graph[i].area);
        printf("%d ", graph[i].area);
    }
    printf("\n");

    int u, v;  // uv 路徑兩端的地點
    for (int i = 0; i < E; i++) {
        fscanf(in, "%d %d ", &u, &v);
        printf("%d %d\n", u, v);
        edge[u][v] = 1;
        edge[v][u] = 1;
    }
    makeAdjList(N + 1, edge);

    for (int i = 0; i < L; i++) {
        pused[i] = 0;
        fscanf(in, "%c%c %d %d ", &plist[i].col[0], &plist[i].col[1], &plist[i].inven, &plist[i].price);
        printf("%c%c %d %d\n", plist[i].col[0], plist[i].col[1], plist[i].inven, plist[i].price);
    }

    algo(0);  //start from graph[0]
    out = fopen("myoutput_3.txt", "w");
    fprintf(out, "%d", gbest);
    printf("\ngbest:%d", gbest);

    return 0;
}