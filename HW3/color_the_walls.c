#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *in;
FILE *out;

char wall[1000000];

void initialize() {
    for (int i = 0; i <= 100000; i++) {
        wall[i] = '_';
    }
}

int main() {
    in = fopen("input_3.txt", "r");
    out = fopen("my_output_3.txt", "w");

    if (in && out) {
        printf("all file open!!!\n\n");

        char line[1000];  // char line[1000];
        int range = 0;
        int ope_num = 0;

        fscanf(in, "%d %d", &range, &ope_num);
        printf("%d\n", range);
        printf("%d\n", ope_num);

        fgets(line, 1000, in);  // \t\n

        initialize();

        for (int i = 0; i < ope_num; i++) {
            char ope, tmp, color;
            int from = 0, to = 0;

            fscanf(in, "%c", &ope);
            printf("%c", ope);

            fscanf(in, "%c", &tmp);  //' '
            printf("%c", tmp);

            fscanf(in, "%d", &from);
            printf("%d ", from);

            fscanf(in, "%d", &to);
            printf("%d", to);

            fscanf(in, "%c", &tmp);  //' '
            printf("%c", tmp);

            if (ope == 'P') {
                fscanf(in, "%c", &color);
                printf("%c\n", color);
                for (int i = from; i <= to; i++) {
                    wall[i] = color;
                }
            }
            else if (ope == 'Q') {
                printf("%c ", wall[from]);
                fprintf(out, "%c ", wall[from]);

                for (int i = from + 1; i <= to; i++) {
                    if (wall[i] != wall[i - 1]) {
                        printf("%c ", wall[i]);
                        fprintf(out, "%c ", wall[i]);
                    }
                }
                fprintf(out, "\n");
            }
            else {
                printf("error operation\n");
            }

            fgets(line, 1000, in);  // \t\n
            printf("\n");
        }
    }
    else {
        printf("open failed!!!\n\n");
    }
}