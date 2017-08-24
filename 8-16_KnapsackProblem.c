/*dynamic programming
0/1 knapsack problem*/
#include<stdio.h>
#include<stdlib.h>
typedef struct {
	int wei;
	int val;
	int is;    // to judage whether to be in the pack
}item;

int maxBackvalue(int v, item *ps, int n);
int main(){
	item ps[4] = { {3,4,0},{8,11,0 },{7,10,0},{9,13,0} };
	int v = 17;
	int num = 4;
	int value;
	value = maxBackvalue(v, ps, num);
	printf("The most value the pack can container is %d.\n",value);
	return 0;
}
/*
knapsack problem
v - volume of the pack
ps - struct array of items
n - numbers of items
out: max value
*/
int maxBackvalue(int v,item *ps,int n){
	int **maxv=(int **)malloc(sizeof(int *)*(n+1));
	for (int i = 0; i <=n; i++)		//dynamic array
		maxv[i] = (int *)malloc(sizeof(int)*(v + 1));
	for (int i = 0; i <= n; i++)
		maxv[i][0] = 0;
	for (int i = 0; i <= v; i++)
		maxv[0][i] = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= v; j++) {
			if (ps[i - 1].wei > j)
				maxv[i][j] = maxv[i - 1][j];
			else {
				maxv[i][j] = max(maxv[i - 1][j], ps[i-1].val + maxv[i - 1][j - ps[i-1].wei]);
			}
		}
	}
	//find the solution
	int v2 = v;
	puts("plan for the pack:");
	for (int i = n; i > 0; i--) {
		if (maxv[i][v2] > maxv[i - 1][v2]) {
			ps[i - 1].is = 1;
			v2 = v2 - ps[i - 1].wei;
			printf("%5d", i);
		}
	}
	printf("\n");
	return maxv[n][v];
	/*
	for (int i = 0; i <= n; i++)
		free(maxv[i]);
	free(maxv);
	*/
}
