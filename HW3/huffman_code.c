#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define QMax 5000

FILE *in;
FILE *out;

typedef struct node *tree_pointer;
typedef struct node {
    char ch;
    int freq;
    tree_pointer left_child;
    tree_pointer right_child;

} Node;
Node data[80];
int cIndex = 0;

tree_pointer root;

int sum = 0;  //answer

/*------------ ⇣ queue ⇣------------*/
int front, rear;
tree_pointer queue[QMax];

int qIsFull() {
    return rear == QMax - 1;  //at the end of the queue
}

int qIsEmpty() {
    if (rear < front)
        return -1;
    else
        return 0;
}

void swap(tree_pointer *a, tree_pointer *b) {
    tree_pointer tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void Enqueue(tree_pointer ele) {
    printf("enqueue %c %d\n", ele->ch, ele->freq);
    if (qIsFull()) {
        printf("%d,the queue is full , can't enqueue\n", rear);
    }
    else {
        rear++;
        queue[rear] = ele;

        if (qIsEmpty()) {
            //printf("-> ele->ch: %c  ele->freq: %d\n", ele->ch, ele->freq);
            //printf("-> front: %d rear:%d\n\n", front, rear);
        }
        else {
            int i = rear;
            //printf("\n");

            while ((i - 1 >= front) && (queue[i]->freq < queue[i - 1]->freq)) {
                swap(&queue[i], &queue[i - 1]);
                i--;
            }
        }
    }
}

tree_pointer Dequeue() {
    if (qIsEmpty()) {
        printf("queue is empty , can't dequeue\n");
        return NULL;
    }
    else {
        tree_pointer ele = queue[front];
        front++;
        //printf("f: %d\tr: %d\n", front, rear);
        return ele;
    }
}

void PrintQueue() {
    for (int i = front; i <= rear; i++) {
        printf("[print=> ele->ch: %c  ele->freq: %d\n", queue[i]->ch, queue[i]->freq);
    }
}
/*------------ ⇡ queue  ⇡ ------------*/

/* ------------⇣ Function ⇣ ------------*/
void initialize() {
    cIndex = 0;
    for (int i = 0; i < 80; i++) {
        data[i].freq = 0;
    }

    front = 0;
    rear = -1;

    sum = 0;
}

void recordData(char c) {
    for (int i = 0; i < cIndex; i++) {
        if (c == data[i].ch) {
            data[i].freq++;
            return;
        }
    }
    data[cIndex].ch = c;
    data[cIndex].freq++;
    cIndex++;
}

void printData() {
    for (int i = 0; i < cIndex; i++) {
        printf("|%c | %d\n", data[i].ch, data[i].freq);
    }
}

void Data_to_Queue() {
    tree_pointer ele = malloc(sizeof(struct node));
    ele->ch = data[0].ch;
    ele->freq = data[0].freq;
    ele->left_child = NULL;
    ele->right_child = NULL;
    printf("\n[0]\n");
    Enqueue(ele);
    PrintQueue();

    for (int i = 1; i < cIndex; i++) {
        if (data[i].ch != data[i - 1].ch) {
            // tree_pointer ele = malloc(sizeof(struct node));
            // ele->ch = data[i].ch;
            // ele->freq = data[i].freq;
            // ele->left_child = NULL;
            // ele->right_child = NULL;

            printf("\n[%d]\n", i);
            //printf("ele->ch: %c  ele->freq: %d\n", ele->ch, ele->freq);
            Enqueue(&data[i]);
            PrintQueue();
        }
    }
}



void Preorder(tree_pointer ptr, int depth) {  //perorder

    if (ptr) {
        if (ptr->left_child == NULL && ptr->right_child == NULL) {
            int level = depth;
            sum += level * ptr->freq;
            printf("%c %d += %d * %d\n", ptr->ch, sum, level, ptr->freq);
        }

        Preorder(ptr->left_child, depth + 1);
        Preorder(ptr->right_child, depth + 1);
    }
}
void PreorderIT(tree_pointer ptr) {  //perorder

    if (ptr) {
        printf("%c%d ", ptr->ch, ptr->freq);

        PreorderIT(ptr->left_child);
        PreorderIT(ptr->right_child);
    }
}

int algo() {
    while (1) {
        tree_pointer ele1 = Dequeue();
        tree_pointer ele2 = Dequeue();

        if (ele2 == NULL) {
            root = ele1;
            printf("root freq: %d\n\n", root->freq);
            break;
        }

        tree_pointer new = malloc(sizeof(struct node));

        new->ch = '/';  //FIXEME 0???
        new->freq = (ele1->freq + ele2->freq);
        new->left_child = ele1;
        new->right_child = ele2;

        Enqueue(new);
    }
    Preorder(root, 0);
    // PreorderIT(root);
    printf("\n");
    return sum;
}

/*------------ ⇡ Function ⇡ ------------*/

int main() {
    in = fopen("input_2.txt", "r");
    out = fopen("my_output_2.txt", "w");

    root = malloc(sizeof(struct node));

    if (in && out) {
        printf("all file open!!!\n\n");
        char line[1000];
        int num = 0;

        fscanf(in, "%d", &num);
        fgets(line, 1000, in);  // \t\n
        printf("------- %d -------\n", num);

        while (num != 0) {
            initialize();
            for (int i = 0; i < num; i++) {
                /*read num lines*/
                fgets(line, 1000, in);
                printf("%s \n", line);

                for (int j = 0; j < strlen(line) - 2; j++) {
                    /*record the sentences*/
                    char c = line[j];
                    recordData(c);
                }
            }
            printData();
            Data_to_Queue();  //push the data into queue
            int ans = algo();
            fprintf(out, "%d\n\n", ans);

            //printf("!!!!!!!!!!ans: %d\n", ans);

            /*read next line*/
            fscanf(in, "%d", &num);
            fgets(line, 1000, in);  // \t\n
            printf("------- %d -------\n", num);
        }
    }
    else {
        printf("open failed!!!\n\n");
    }
    return 0;
}