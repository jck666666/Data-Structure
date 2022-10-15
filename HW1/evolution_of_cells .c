#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define Max_size 500

int arr[Max_size][Max_size];
int nextarr[Max_size][Max_size];

void evolution(int, int);
void printMatrix(int); //print what I read in input_2.txt
int neighbors(int, int, int);
void renew(int);
void output(int, int);
int nonzero(int);

void printMatrix(int size)
{
	printf("size:%d\n", size);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("|%d", arr[i][j]);
		}
		printf("\n");
	}
}
void evolution(int size, int generations)
{
	for (int gen = 0; gen < generations; gen++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				switch (neighbors(i, j, size))
				{
				case 2:
					nextarr[i][j] = arr[i][j];
					break;
				case 3:
					nextarr[i][j] = 1;
					break;
				default: //0,1,4,5,6,7,8
					nextarr[i][j] = 0;
				}
			}
		}

		/*	renew current arr	(copy nextarr[][] to arr[][])	*/
		renew(size);
	}
}

int neighbors(int cur_row, int cur_col, int size)
{
	int live = 0; //count the neighbors which is live
	int dir[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

	for (int i = 0; i < 8; i++) //8 directions
	{
		int row = cur_row + dir[i][0];
		int col = cur_col + dir[i][1];

		if (row >= 0 && row < size && col >= 0 && col < size)
		{
			if (arr[row][col] == 1)
			{
				live++;
			}
		}
	}

	return live;
}

void renew(int size)
{
	/*	copy nextarr[][] to arr[][]	*/

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			arr[i][j] = nextarr[i][j];
			nextarr[i][j] = 0;
		}
	}
}

void output(int size, int flag)
{
	/*	output to  "output_2.txt"	*/
	FILE *fp;
	if (flag == 0)
	{
		fp = fopen("output_2.txt", "w");
		if (fp == NULL)
		{
			printf("can't open output_2.txt");
		}
		else
		{
			printf("open success!!!!\n");
		}
	}
	else if (flag == 1)
	{
		fp = fopen("output_2.txt", "a");
		if (fp == NULL)
		{
			printf("can't open output_2.txt");
		}
		else
		{
			printf("open success!!!!\n");
			/* Use the sparse matrix to output 	*/
			fprintf(fp, "%d %d %d\n", size, size, nonzero(size));

			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					if (arr[i][j])
					{
						fprintf(fp, "%d %d %d\n", i, j, neighbors(i, j, size));
					}
				}
			}
		}
	}

	fclose(fp);
}

int nonzero(int size)
{
	int cnt = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (arr[i][j])
				cnt++;
		}
	}
	return cnt;
}

int main()
{
	/*	read "input_2.txt"	*/
	FILE *fp = fopen("input_2.txt", "r");
	if (fp == NULL)
	{
		printf("can't open input_2.txt");
	}

	int testdata = 0, size = 0, generations = 0;

	/*	first line(total matrix test data) in input_2.txt	*/
	fscanf(fp, "%d", &testdata);
	output(size, 0); //open a new output file

	for (int num = 0; num < testdata; num++)
	{
		/*	size : the size of the array is N*N
		generations : how many generations that we will simulate	*/
		fscanf(fp, "%d %d", &size, &generations);

		for (int i = 0; i < size; i++) //row
		{
			for (int j = 0; j < size; j++) //col
			{
				fscanf(fp, "%d", &arr[i][j]);
			}
		}

		/*	(before evolution)	*/
		printMatrix(size);

		/*	simulate the evolution of cells	*/
		evolution(size, generations);

		/*	(after evolution)	*/
		printMatrix(size);

		/* Use the sparse matrix to output 	*/
		output(size, 1);
	}

	fclose(fp);

	return 0;
}