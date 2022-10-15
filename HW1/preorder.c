#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#define OPNUM 12
#define MAX_LENGTH 200
#define EXPRE_NUM 200 //expression

char operator[12] = {'(', ')', '*', '%', '/', '+', '-', '>', '<', '&', '^', '|'};
int priority[12] = {0, 0, 13, 13, 13, 12, 12, 10, 10, 8, 7, 6};

int top = -1;
char data[EXPRE_NUM] = {};
int i;

/* ---------↓ function ↓-----------*/

void Out(char *str, int flag)
{
    /* output to  "output_1.txt" */
    FILE *fp;
    if (flag == 0)
    {
        fp = fopen("output_1.txt", "w");
        if (fp == NULL)
        {
            printf("can't open output_1.txt");
        }
        else
        {
            printf("open success!!!!\n");
        }
    }
    else if (flag == 1)
    {
        fp = fopen("output_1.txt", "a");
        if (fp == NULL)
        {
            printf("can't open output_1.txt");
        }
        else
        {
            printf("open success!!!!\n");
            fputs(str, fp);
        }
    }

    fclose(fp);
}

bool IsOperator(char c)
{
    for (int i = 0; i < OPNUM; i++)
    {
        if (c == operator[i])
            return true;
    }
    return false;
}

void reverse(char *str, int from, int to)
{
    char c;

    while (from <= to) //<=?? <???
    {
        c = str[from];
        str[from++] = str[to];
        str[to--] = c;
    }
}

int Priority(char c)
{
    int i;

    for (i = 0; i < OPNUM; i++)
    {
        if (operator[i] == c)
            return priority[i];
    }
    return -1;
}

void Prefix(char *str, int from, int to)
{

    char stack[MAX_LENGTH] = {'\0'};
    char out_stack[MAX_LENGTH] = {'\0'};
    int i, index = 0, top = -1;

    for (i = 0; i < to; i++)
    {
        int next = i + 1;
        if (!IsOperator(str[i])) //operand
        {
            out_stack[index++] = str[i];
        }
        else if (str[i] == '-' && IsOperator(str[next])) //negative number
        {
            out_stack[index++] = str[i];
        }
        else //is operator
        {
            switch (str[i])
            {
            case ')':
                stack[++top] = str[i];
                break;

            case '*':
            case '%':
            case '/':
            case '+':
            case '-':
            case '>':
            case '<':
            case '&':
            case '^':
            case '|':
                while (Priority(stack[top]) > Priority(str[i]))
                {
                    out_stack[index++] = ' '; //front of operator
                    out_stack[index++] = stack[top--];
                }
                stack[++top] = str[i];    // push into stack
                out_stack[index++] = ' '; //rear of operator
                break;

            case '(': //pop all of the items that are in stack before match ')'
                while (stack[top] != ')')
                {
                    out_stack[index++] = ' '; //balnk between every operator
                    out_stack[index++] = stack[top--];
                }
                top--; // don't push ')' into out_stack[]
                break;

            default:
                printf("error is here~~~");
            }
        }
    }
    while (top > -1) //if there is sth left in stack,clean the stack
    {
        out_stack[index++] = ' ';
        out_stack[index++] = stack[top--];
    }

    //pop the items in out_stack[] and restore it into str[i]
    i = 0;
    while (index > -1)
    {
        str[i++] = out_stack[--index];
    }
}

/* ---------↑ function ↓-----------*/
int main()
{

    FILE *fp = fopen("input_1.txt", "r");
    if (fp == NULL)
    {
        printf("can't open input_1.txt");
    }
    else
    {
        printf("open!\n");
        Out("", 0);

        while (!feof(fp))
        {
            char c = fgetc(fp);

            if (c == '\n' || c == '\0')
            {
                
                /* reverse */
                reverse(data, 0, strlen(data) - 1); //-1??
                                                    

                /* prefix */
                Prefix(data, 0, strlen(data)); //only one prefix operation at a time
               

                /* output */
                Out(data, 1);
                Out("\n", 1);

                i = 0;
                for (int j = 0; j < MAX_LENGTH; j++)
                {
                    data[j] = '\0';
                }
                continue;
            }
            data[i++] = c;
        }
    }

    fclose(fp);
}