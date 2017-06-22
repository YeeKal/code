#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
int main()
{
	int i, j;
	int x = 1;
	int y = 1;
	int velocityX = 1, velocityY = 1;
	int height = 20, width = 30,top=1,left=1;
	while(1)
	{
		x = x + velocityX;
		y = y + velocityY;
		system("cls");//clear screen
		//输出上面空行
		for (i = 0; i < x; i++)
		{
			printf("\n");
		}
		//输出小球左边空格
		for (j = 0; j < y; j++)
		{
			printf("  ");
		}
		printf("O");
		printf("\n");
		Sleep(50);//wait for seconds
		if (x == height||x==top) {
			velocityX = -velocityX;
			printf("\a");//ring when knocking the edge
		}
		if (y == width || y == left) {
			velocityY = -velocityY;
			printf("\a");
		}

	}
}