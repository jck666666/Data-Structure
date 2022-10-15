#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 20

FILE *INorders;
FILE *INrecipes;
FILE *OUTplayers;

int balabce = 0;
char ingredients[MAX];
int queue[1000];  //store the index of order
int qlen = 0;

typedef enum {
    false,
    true
} bool;
bool P1_busy[1000];
bool P2_busy[1000];
bool table_busy[1000];

/*  -----------struct ----------- */
typedef struct recipes {
    char recipe_name[MAX];
    char s[MAX][MAX];  //cook
    int snum;
    char c[MAX][MAX];  //cut
    int cnum;
    char o[MAX][MAX];
    int onum;
} recipes;  //recipes
recipes r[1000];
int r_total;

typedef struct orders {
    char order_id[5];
    char recipe_name[MAX];
    int arrival;
    int deadline;
    int money;
    int punishment;
    int sleft;
    int cleft;
    int fleft;
    int f_time_need;
    int state;  //-1:unarrived 0:can_do_it 1:over_deadline
    float urgent;
} orders;  //orders
orders o[1000];
int o_total;
bool finish_order[1000];

typedef struct players {
    char player_id;
    int t;
    char order_id[5];
    char command;
    char object[MAX];

} players;  //players
players p[1000];
int work = 0;
/* -----------struct----------- */

void insertion_sort(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int j = i;
        while (j > 0 && o[arr[j - 1]].urgent > o[arr[j]].urgent) {
            int temp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = temp;
            j--;
        }
    }
}

int find_recipes_index(char *recipe_name) {
    for (int i = 0; i < r_total; i++) {
        if (!strcmp(recipe_name, r[i].recipe_name)) {
            return i;
        }
    }
    return 9999;
}

int timeNeed(char *order_id) {
    int need = 0;

    for (int i = 0; i < o_total; i++) {
        if (!strcmp(order_id, o[i].order_id)) {
            int index = find_recipes_index(o[i].recipe_name);  //找到此訂單的作法 -> 食譜中的第幾道
            need += r[index].snum * 5;
            need += r[index].cnum * 3;
            need += r[index].onum * 1;
            need += 1;  //presentation
            return need;
        }
    }
    return 9999;
}

int orderArr(int moment) {
    for (int seq = 0; seq < o_total; seq++) {
        if (o[seq].arrival == moment)
            return seq;
    }
    return -1;
}

void build_order_progress() {
    for (int i = 0; i < o_total; i++) {
        o[i].f_time_need = 1 + r[find_recipes_index(o[i].recipe_name)].onum;
        o[i].sleft = r[find_recipes_index(o[i].recipe_name)].snum;
        o[i].cleft = r[find_recipes_index(o[i].recipe_name)].cnum;
        o[i].fleft = 1;
    }
}

void renewState(int moment) {
    for (int i = 0; i < o_total; i++) {
        if (o[i].arrival <= moment && moment <= o[i].deadline) {
            o[i].state = 0;
        }
        else if (o[i].deadline < moment) {
            o[i].state = 1;
        }
    }
}

void Priority() {
    for (int i = 0; i < o_total; i++) {
        o[i].urgent = -1;
        if (o[i].state == 0) {
            o[i].urgent = (float)(o[i].money - o[i].punishment) ;
            //o[i].urgent = (float)(o[i].money - o[i].punishment) / (float)(o[i].deadline - o[i].arrival);
        }
    }
}

int available_player(int moment) {
    int who;
    if (P1_busy[moment] == true && P2_busy[moment] == true) {
        who = -1;  //neither available
    }
    else if (P1_busy[moment] == true && P2_busy[moment] == false)  //P2 availiable
    {
        who = 2;
    }
    else if (P1_busy[moment] == false && P2_busy[moment] == true)  //P1 availiable
    {
        who = 1;
    }
    else  //both availiable
    {
        who = 2;
    }

    return who;
}

int f_checkTime(char *order_id) {
    for (int i = work - 1; i >= 0; i--) {
        int time;
        if (!strcmp(p[i].order_id, order_id)) {
            if (p[i].command == 's') {
                time = p[i].t + 5;
            }
            else if (p[i].command == 'c') {
                time = p[i].t + 3;
            }

            return time;
        }
    }
    return 99999;
}

void renewQueue() {
    qlen = 0;

    for (int i = 0; i < 1000; i++)  //initialize
    {
        queue[i] = 0;
    }

    for (int i = 0; i < o_total; i++) {
        if (o[i].state == 0 && o[i].fleft == 1)  //cando && haven't done
        {
            queue[qlen++] = i;
        }
    }

    insertion_sort(queue, qlen);  //sort the order in queue by urgent
    for (int i = qlen - 1; i >= 0; i--) {
        printf("qlen %d: %s |urgent: %f\n", i, o[queue[i]].order_id, o[queue[i]].urgent);
    }
}

int find_max_deadline() {
    int max = 0;
    for (int i = 0; i < o_total; i++) {
        if (o[i].deadline > max) {
            max = o[i].deadline;
        }
    }
    return max;
}

void algo() {
    int endtime = find_max_deadline();
    int seq;
    int who;

    for (int now = 0; now <= endtime; now++)  //time line
    {
        int parrellel = 1;
        while (parrellel++ <= 2)  //每個時間點讓他跑兩次（給兩個玩家在這個時間點工作的機會）
        {
            printf("\n\ntime: %d\n", now);
            printf("---P1 is %d---\n", P1_busy[now]);
            printf("---P2 is %d---\n", P2_busy[now]);
            printf("---table is %d---\n", table_busy[now]);

            renewState(now);
            Priority();
            renewQueue();

            // for (int m = qlen - 1; m >= 0; m--)
            // {
            int qindex = qlen - 1;
            while ((o[queue[qindex]].fleft == 0) || (o[queue[qindex]].sleft == 0 && o[queue[qindex]].cleft == 0 && now < f_checkTime(o[queue[qindex]].order_id))) {
                qindex--;
                if (qindex == 0 || queue[qindex] <= 0) {
                    break;
                }
            }
            int i = queue[qindex];

            if (o[i].state == 0 && o[i].fleft != 0)  //cando && haven't done
            {
                printf("going to do %s\n", o[queue[qindex]].order_id);
                printf("\nID:%s arr:%d dead:%d state:%d urgent:%f\n", o[i].order_id, o[i].arrival, o[i].deadline, o[i].state, o[i].urgent);

                who = available_player(now);

                if (who == 1 || who == 2)  //someone is available
                {
                    printf("who can be dispatched:%d \n", who);
                    int r_seq = find_recipes_index(o[i].recipe_name);

                    if (o[i].sleft > 0 && table_busy[now] == false && now + 5 <= o[i].deadline) {
                        printf("...stove...\n");
                        int ingredients_seq = o[i].sleft - 1;

                        sprintf(ingredients, "%s", r[r_seq].s[ingredients_seq]);  //那道菜的某動作之剩餘食材
                        o[i].sleft--;
                        printf("sleft: %d\n", o[i].sleft);
                        printf("cleft: %d\n", o[i].cleft);

                        p[work].command = 's';
                        sprintf(p[work].object, "%s", ingredients);

                        int cnt = 5;
                        int tmp = now;
                        while (cnt-- > 0) {
                            if (who == 1)

                                P1_busy[tmp] = true;
                            else if (who == 2)
                                P2_busy[tmp] = true;

                            table_busy[tmp++] = true;
                        }
                    }
                    else if (o[i].cleft > 0 && table_busy[now] == false && now + 3 <= o[i].deadline) {
                        printf("...cutting...\n");
                        int ingredients_seq = o[i].cleft - 1;

                        sprintf(ingredients, "%s", r[r_seq].c[ingredients_seq]);  //那道菜的某動作之剩餘食材
                        o[i].cleft--;
                        printf("sleft: %d\n", o[i].sleft);
                        printf("cleft: %d\n", o[i].cleft);

                        p[work].command = 'c';
                        sprintf(p[work].object, "%s", ingredients);

                        int cnt = 3;
                        int tmp = now;
                        while (cnt-- > 0) {
                            if (who == 1)
                                P1_busy[tmp] = true;
                            else if (who == 2)
                                P2_busy[tmp] = true;

                            table_busy[tmp++] = true;
                        }
                    }
                    else if (o[i].sleft == 0 && o[i].cleft == 0 && o[i].fleft == 1 && (now + o[i].f_time_need <= o[i].deadline))  //剩擺盤
                    {
                        if (now < f_checkTime(o[i].order_id)) {
                            printf("...wait,can't presentation...\n");
                            //此時的時間要 >= 此訂單擺盤前的那個動作時間+他需要的時間 才能擺盤
                            continue;  //還不能擺盤
                        }
                        printf("...can presentation...\n");
                        printf("@@@ f need time to: %d\n", now + o[i].f_time_need);
                        finish_order[i] = true;
                        o[i].fleft = 0;
                        p[work].command = 'f';
                        printf("!!!!!!!!!!!!!!!!!!!!!!\n");
                        sprintf(p[work].object, "%s", " ");

                        int cnt = o[i].f_time_need;
                        int tmp = now;
                        while (cnt-- > 0) {
                            if (who == 1)
                                P1_busy[tmp] = true;
                            else if (who == 2)
                                P2_busy[tmp] = true;

                            tmp++;
                        }
                    }
                    else {
                        printf("...nothing...\n");
                        break;
                    }

                    p[work].player_id = who;
                    p[work].t = now;
                    sprintf(p[work].order_id, "%s", o[i].order_id);

                    printf("%d %d %s %c %s | work cnt: %d\n", p[work].player_id, p[work].t, p[work].order_id, p[work].command, p[work].object, work);
                    work++;
                }
                else {
                    printf("no one is  available ~ \n");
                }
            }
            else {
            }
            //}
        }
    }
}

int main() {
    INorders = fopen("orders.txt", "r");
    INrecipes = fopen("recipes.txt", "r");
    OUTplayers = fopen("players.txt", "w");

    char string[MAX];
    char tmpc;

    if (INorders == NULL && INrecipes == NULL && OUTplayers == NULL) {
        printf("can't open all file\n\n");
    }
    else {
        printf("all file open success\n\n");

        /* ⇣⇣⇣read  orders⇣⇣⇣ */
        fscanf(INorders, "%d", &o_total);
        for (int i = 0; i < o_total; i++) {
            o[i].state = -1;  //default : haven't arrived
            fscanf(INorders, "%s", string);
            sprintf(o[i].order_id, "%s", string);

            fscanf(INorders, "%s", string);
            sprintf(o[i].recipe_name, "%s", string);

            fscanf(INorders, "%d", &o[i].arrival);
            fscanf(INorders, "%d", &o[i].deadline);
            fscanf(INorders, "%d", &o[i].money);
            fscanf(INorders, "%d", &o[i].punishment);
        }
        /* ⇡⇡⇡read  orders ⇡⇡⇡ */

        /* ⇣⇣⇣read  recipes⇣⇣⇣ */
        int j = 0, index = 0;
        fscanf(INrecipes, "%d", &r_total);
        for (int i = 0; i < r_total; i++) {
            fscanf(INrecipes, "%s", string);
            sprintf(r[i].recipe_name, "%s", string);
            printf("-%s- ", r[i].recipe_name);

            fscanf(INrecipes, "%c", &tmpc);

            /* --------------------s --------------------*/
            j = 0, index = 0;
            r[i].snum = 0;
            fscanf(INrecipes, "%c", &tmpc);
            do {
                if (tmpc == ',') {
                    printf("%s,", r[i].s[j]);
                    if (strcmp(r[i].s[j], "x")) {
                        r[i].snum++;
                    }
                    j++;
                    index = 0;
                }
                else {
                    r[i].s[j][index++] = tmpc;
                }

                fscanf(INrecipes, "%c", &tmpc);

            } while (tmpc != ' ');
            if (strcmp(r[i].s[j], "x")) {
                r[i].snum++;
            }
            printf("%s%d ", r[i].s[j], r[i].snum);

            /*-------------------- c-------------------- */
            j = 0, index = 0;
            r[i].cnum = 0;
            fscanf(INrecipes, "%c", &tmpc);
            do {
                if (tmpc == ',') {
                    if (strcmp(r[i].c[j], "x")) {
                        r[i].cnum++;
                    }
                    printf("%s,", r[i].c[j]);
                    j++;
                    index = 0;
                }
                else {
                    r[i].c[j][index++] = tmpc;
                }

                fscanf(INrecipes, "%c", &tmpc);

            } while (tmpc != ' ');
            if (strcmp(r[i].c[j], "x")) {
                r[i].cnum++;
            }
            printf("%s%d ", r[i].c[j], r[i].cnum);

            /*-------------------- o--------------------*/

            j = 0, index = 0;
            r[i].onum = 0;
            fscanf(INrecipes, "%c", &tmpc);
            do {
                if (tmpc == ',') {
                    if (strcmp(r[i].o[j], "x")) {
                        r[i].onum++;
                    }
                    printf("%s,", r[i].o[j]);
                    j++;
                    index = 0;
                }
                else {
                    r[i].o[j][index++] = tmpc;
                }

                fscanf(INrecipes, "%c", &tmpc);

            } while (tmpc != '\r' && tmpc != '\n' && !feof(INrecipes));

            if (strcmp(r[i].o[j], "x")) {
                r[i].onum++;
            }
            printf("%s%d\n", r[i].o[j], r[i].onum);
        }
        /* ⇡⇡⇡read  recipes end ⇡⇡⇡*/

        /* ----------------⇣⇣⇣ my algorithm ⇣⇣⇣-------------- */
        build_order_progress();
        Priority();

        algo();

        /* ----------------⇣⇣⇣ output players.txt⇣⇣⇣-------------- */
        printf("%d\n", work);
        fprintf(OUTplayers, "%d\n", work);
        for (int i = 0; i < work; i++) {
            printf("%d %d %s %c %s\n", p[i].player_id, p[i].t, p[i].order_id, p[i].command, p[i].object);
            fprintf(OUTplayers, "%d %d %s %c %s\n", p[i].player_id, p[i].t, p[i].order_id, p[i].command, p[i].object);
        }

        /* ----------------⇣⇣⇣ earn ⇣⇣⇣-------------- */
        printf("\ninitial balance: %d \n\n", balabce);
        for (int i = 0; i < o_total; i++) {
            if (finish_order[i] == true) {
                printf("balance: %d \n", balabce);
                balabce += o[i].money;
                printf("%s : +%d \n", o[i].order_id, o[i].money);
            }
            else {
                printf("balance: %d \n", balabce);
                balabce += o[i].punishment;
                printf("%s : %d \n", o[i].order_id, o[i].punishment);
            }
            printf("balance: %d \n\n", balabce);
        }
        printf("final balabce: %d\n", balabce);

        // for (int i = 0; i < o_total; i++) {
        //     if (finish_order[i] == true) {
        //         printf("%s\n", o[i].order_id);
        //     }
        // }
    }

    fclose(INorders);
    fclose(INrecipes);
    fclose(OUTplayers);
    return 0;
}
// INorders = fopen("/Users/jck/Desktop/DS\ HW2/homework_2/orders.txt", "r");
// INrecipes = fopen("/Users/jck/Desktop/DS\ HW2/homework_2/recipes.txt", "r");
// OUTplayers = fopen("/Users/jck/Desktop/DS\ HW2/homework_2/players.txt", "w");