#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define Max_size 100

int map[Max_size][Max_size];

typedef struct point
{
	int x;
	int y;
} Point;

Point start_p;		 //starting point
Point dest_p_ary[5]; //destination point
Point queue[Max_size * Max_size];

int front, rear; //index of queue

int qIsFull()
{
	return rear == (Max_size * Max_size) - 1; //at the end of the queue
}

int qIsEmpty()
{
	if (rear < front)
		return -1;
	else
		return 0;
}

void enqueue(int x, int y)
{
	if (qIsFull())
	{
		printf("%d,the queue is full , can't enqueue\n", rear);
	}
	else
	{
		rear++;
		queue[rear].x = x;
		queue[rear].y = y;
		// printf("f: %d\tr: %d\n", front, rear);
	}
}
Point dequeue()
{
	if (qIsEmpty())
	{
		printf("queue is empty , can't dequeue\n");
	}
	else
	{
		Point cur = queue[front];
		front++;
		//printf("f: %d\tr: %d\n", front, rear);
		return cur;
	}
}

// void initdp(int size, int **dp)
// {
//     for (int i = 0; i < size; i++)
//     {
//         for (int j = 0; j < size; j++)
//         {
//             dp[i][j] = INT_MAX;
//         }
//     }
// }
int max(int i, int j)
{
	if (i > j) //很明顯的比大小了
		return i;
	else
		return j;
}

int path(int destNum, int size)
{
	int cost = 0;
	int dir[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
	Point target_start = start_p;

	for (int d = 0; d < destNum; d++)
	{
		Point target_dest = dest_p_ary[d];
		bool visited[Max_size][Max_size] = {};
		int dp[size][size]; //dynamic program
		//----------------------------------------------------------------------------------
		front = 0, rear = -1; //initialize the index in queue
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				dp[i][j] = INT_MAX;
				visited[i][j] = false;
			}
		}

		dp[target_start.x][target_start.y] = 0;
		visited[target_start.x][target_start.y] = true;

		enqueue(target_start.x, target_start.y);

		while (!qIsEmpty())
		{
			Point cur = dequeue();

			/* clockwise */
			for (int i = 0; i < 8; i++)
			{
				Point next;
				next.x = cur.x + dir[i][0];
				next.y = cur.y + dir[i][1];

				if (next.x >= 0 && next.x < size && next.y >= 0 && next.y < size) //not out of bound
				{
					if (visited[next.x][next.y] == false)
					{
						visited[next.x][next.y] = true;
						enqueue(next.x, next.y);
					}

					int height_diff = abs(map[cur.x][cur.y] - map[next.x][next.y]);
					if (dp[cur.x][cur.y] + height_diff <= dp[next.x][next.y])
					{
						dp[next.x][next.y] = (dp[cur.x][cur.y] + height_diff);
					}
				}
			}
		}

		//----------------------------------------------------------------------------------
		front = 0, rear = -1; //initialize the index in queue
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				visited[i][j] = false;
			}
		}
		dp[target_start.x][target_start.y] = 0;
		visited[target_start.x][target_start.y] = true;

		enqueue(target_start.x, target_start.y);
		while (!qIsEmpty())
		{
			Point cur = dequeue();

			/* Counterclockwise */
			for (int i = 7; i >= 0; i--)
			{
				Point next;
				next.x = cur.x + dir[i][0];
				next.y = cur.y + dir[i][1];

				if (next.x >= 0 && next.x < size && next.y >= 0 && next.y < size) //not out of bound
				{
					if (visited[next.x][next.y] == false)
					{
						visited[next.x][next.y] = true;
						enqueue(next.x, next.y);
					}

					int height_diff = abs(map[cur.x][cur.y] - map[next.x][next.y]);
					if (dp[cur.x][cur.y] + height_diff <= dp[next.x][next.y])
					{
						dp[next.x][next.y] = (dp[cur.x][cur.y] + height_diff);
					}
				}
			}
		}
		//----------------------------------------------------------------------------------

		printf("Part Of Cost: %d\n", dp[target_dest.x][target_dest.y]);
		cost += dp[target_dest.x][target_dest.y];

		target_start = target_dest;
	}

	return cost;
}

int main()
{
	FILE *in = fopen("input_3.txt", "r");
	FILE *out = fopen("output_3.txt", "w");

	if (in == NULL)
	{
		printf("can't open input_3.txt");
	}

	int testdata = 0, destNum = 0, size = 0;

	/*	first line(total test data) in input_3.txt	*/
	fscanf(in, "%d", &testdata);

	for (int num = 0; num < testdata; num++)
	{
		printf("----  #%d  ----\n", num + 1);

		fscanf(in, "%d %d", &destNum, &size);
		fscanf(in, "%d %d", &start_p.x, &start_p.y);

		/*	store destination point	*/
		for (int i = 0; i < destNum; i++)
		{
			fscanf(in, "%d %d", &dest_p_ary[i].x, &dest_p_ary[i].y);
			printf("%d %d\n", dest_p_ary[i].x, dest_p_ary[i].y);
		}

		/*	store map	*/
		for (int i = 0; i < size; i++) //row
		{
			for (int j = 0; j < size; j++) //col
			{
				fscanf(in, "%d", &map[i][j]);
				printf("%d\t ", map[i][j]);
			}
			printf("\n");
		}

		int cost = path(destNum, size);
		printf("cost: %d\n", cost);
		fprintf(out, "#%d\ncost:%d\n", num + 1, cost);
	}

	fclose(in);
	fclose(out);
	return 0;
}
