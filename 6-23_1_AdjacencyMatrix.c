/*graph with adjacency matrix*/
#include<stdio.h>
#include<stdlib.h>

void printGraph(int **ps, int v);

int main() {
	int v, m,n;
	puts("Enter the number of vertices:");
	scanf("%d", &v);
	int **ps;
	//2d memory allocation
	ps = (int **)malloc(v*sizeof(int *));
	for (int i = 0; i < v; i++) {
		ps[i] = (int *)malloc(v*sizeof(int));
	}
	//initialize
	for (int i = 0; i < v; i++) {
		for (int j = 0; j < v; j++)
			ps[i][j] = 0;
	}
	for (int i = 0; i < v; i++)
		ps[i][i] = 1;
	//connect
	puts("enter two num to connect and non-num to quit:");
	while (scanf("%d %d", &m, &n) == 2) {
		ps[m][n] = ps[n][m] = 1;
		printGraph(ps, v);	
	}
	return 0;
}

void printGraph(int **ps, int v) {
	//col num
	printf("   ");
	for (int i = 0; i < v; i++)
		printf("%4d", i);
	//line
	printf("\n---");
	for (int i = 0; i < v; i++)
		printf("----", i);
	printf("\n");
	//row num and value
	for (int i = 0; i < v; i++) {
		printf("%-2d|",i);
		for (int j = 0; j < v; j++)
			printf("%4d", ps[i][j]);
		putchar('\n');
	}
}
