#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *in;
FILE *out;
int cnt = 0;

typedef struct listNode *listPointer;
typedef struct listNode
{
    int data;
    listPointer link;
};

/* ------------⇣operation func⇣ ------------*/
void add(listPointer *first, listPointer *trail, int x)
{
    cnt++;
    listPointer last;
    listPointer new = malloc(sizeof(struct listNode));
    if (new != NULL)
    {
        new->data = x;
        new->link = NULL;

        if (!(*first)) //empty list
        {
            *first = new;
        }
        else
        {

            last = (*first);
            while (last->link != NULL) //if the next node is exist
            {
                last = last->link;
            }
            last->link = new;
        }
        last = new;
        printf("last: %d ", last->data);
        //*trail = new;
    }
    else
    {
        printf("%d not inserted.No memory available.\n,", x);
    }
}

void del(listPointer *first, listPointer *trail, int x)
{
    if (x > cnt) //over the list,don't do anything
        return;
    cnt--;
    listPointer pre, target;
    target = pre = (*first); //default

    if (x == 1)
    {
        (*first) = (*first)->link;
    }
    else if (x == 2)
    {
        (*first)->link = (*first)->link->link;
    }
    else
    {
        for (int i = 0; i < x - 2; i++)
        {
            pre = pre->link;
        }

        if (pre != target) //target is not the 1st node
        {
            target = pre->link;
            pre->link = target->link;
        }
        else //target is the 1st node
        {
            (*first) = target->link;
        }
        free(target);
    }
}

void ins(listPointer *first, listPointer *trail, int x, int y)
{
    if (x > cnt)
        return;
    cnt++;
    listPointer new = malloc(sizeof(struct listNode));
    listPointer pre;
    pre = (*first); //default

    for (int i = 1; i < x; i++)
    {
        pre = pre->link;
    }
    new->data = y;
    new->link = pre->link;
    pre->link = new;
}

void mul(listPointer *first, listPointer *trail, int x, int y)
{
    if (x > cnt)
        return;
    listPointer target, explore;
    explore = (*first);
    target = (*first);

    while (x-- > 0) //explore set off
    {
        explore = explore->link;
    }
    while (explore->link != NULL) //explore && explore->link != NULL
    {
        explore = explore->link;
        target = target->link;
    }
    target = target->link;
    target->data = (target->data) * y;
}

listPointer rev(listPointer *first, listPointer *trail, int x)
{
    listPointer cur, next, pre;
    cur = (*first);
    next = NULL;
    pre = NULL;

    if (x >= cnt) //reverse the list
    {
        x = cnt;
        int count = 0;
        while (count < x && cur != NULL)
        {
            next = cur->link;
            cur->link = pre;
            pre = cur;
            cur = next;
            count++;
        }
        (*first) = pre;
        return (*first);
    }

    if (!(*first)) //the list is not exist
    {
        return NULL;
    }

    int count = 0;
    while (count < x && cur != NULL)
    {
        next = cur->link;
        cur->link = pre;
        pre = cur;
        cur = next;
        count++;
    }

    if (next != NULL) //next is exist
    {
        printf(" %d ", (*first)->data);
        (*first)->link = rev(&next, &trail, x);

        (*first) = pre;
    }

    return pre;
}

void show(listPointer first) //為什麼印 list 不用指標
{
    listPointer current;
    printf("\n--list-- %d \n", cnt);
    for (current = first; current; current = current->link)
    {
        printf("%d -> ", current->data);
        fprintf(out, "%d ", current->data);
    }
    printf("NULL\n\n");
    fprintf(out, "\n");
}

/*------------ ⇡operation func⇡ ------------*/

int main()
{
    listPointer first, trail;
    first = trail = NULL;

    in = fopen("input_1.txt", "r");
    out = fopen("my_output_1.txt", "w");

    if (in == NULL)
    {
        printf("can't open input_1.txt\n\n");
    }
    else
    {
        printf("input_1.txt open!!!\n\n");

        if (out == NULL)
        {
            printf("can't open my_output_1.txt\n\n");
        }
        else
        {
            printf("my_output_1.txt open!!!\n\n");

            /* read the initial list */

            int num = 0;
            char tmp;
            do
            {
                fscanf(in, "%d", &num);
                add(&first, &trail, num);
                fscanf(in, "%c", &tmp); //' ' or '\n'

            } while (tmp == ' ' && tmp != '\n');

            //show(first);

            int task;
            fscanf(in, "%d", &task);
            printf("\n%d\n", task);

            char line[300];
            char op[20];
            int x = 0, y = 0;
            fgets(line, 20, in); //read the previous line
            for (int i = 0; i < task; i++)
            {
                if (fgets(line, 20, in) != NULL)
                {
                    printf("%s", line);
                    sscanf(line, "%s", op);

                    if (strcmp(op, "add") == 0)
                    {
                        sscanf(line, "%s %d", op, &x);
                        add(&first, &trail, x);
                    }
                    else if (strcmp(op, "del") == 0)
                    {
                        sscanf(line, "%s %d", op, &x);
                        del(&first, &trail, x);
                    }
                    else if (strcmp(op, "ins") == 0)
                    {
                        sscanf(line, "%s %d %d", op, &x, &y);
                        ins(&first, &trail, x, y);
                    }
                    else if (strcmp(op, "mul") == 0)
                    {
                        sscanf(line, "%s %d %d", op, &x, &y);
                        mul(&first, &trail, x, y);
                    }
                    else if (strcmp(op, "rev") == 0)
                    {
                        sscanf(line, "%s %d", op, &x);
                        rev(&first, &trail, x);
                    }
                    else if (strcmp(op, "show") == 0)
                    {
                        show(first);
                    }
                    else
                    {
                        printf("\n------error function------\n");
                    }
                    //show(first); //for check in every operation
                }
            }
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}
