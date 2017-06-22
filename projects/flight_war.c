#include<stdio.h>
#include<stdlib.h>
#include<conio.h> //�������
#include<windows.h>
#include<time.h>
void undateUser(void);
void show(void);
void updateAction(void);
void gotoxy(int, int);

//����ȫ�ֱ���
int i, j;
int left = 20, height = 10;
int isFire = 0;
int barY = 10, barX = 0, cycle = 0, cycleEnd = 10;
int isKilled = 0;
int score = 0;
char move;



void gotoxy(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}
void updateUser() {
	if (_kbhit()) {//������Ϊ1������Ϊ0
		move = _getch();//��ȡ����ֵ
		switch (move)
		{
		case 'A':
			left--;
			break;
		case 'W':
			height--;
			break;
		case 'S':
			height++;
			break;
		case 'D':
			left++;
			break;
		case ' ':
			isFire = 1;
			break;
		default:
			break;
		}
	}
}
void updateAction() {
	if(isKilled){		//������÷֣�x���������㣬y�������������
		score++;
		isKilled = 0;
		height = height + barX;
		barY = (rand() % 40 + 2);
		barX = 0;
	}
}
void show() {
	//gotoxy(0, 0);//����ƶ���ԭ���ػ�
	system("cls");
	cycle++;

	if (cycle == cycleEnd) {	//ѭ��10������һ��
		barX++;
		height--;		//�ɻ��߶������һ
		if (height == 0) {
			printf("Game over!\n");
			printf("press any key to exit...\n");
			getchar();
			exit(0);
		}
		cycle = 0;
	}
	printf("      your score:%d\n", score);
	//Sleep(150);
	//��ʾ�л�
	if (!isKilled) {
		for (i = 0; i < barX; i++) {
			printf("\n");
		}
		for (j = 0; j < barY; j++) {
			printf(" ");
		}
		printf("+\n");
	}
	

	//�Ƿ��伤����
	if (isFire == 0) {
		for (i = 0; i < height; i++) {
			printf("\n");
		}
	}
	else {
		for (i = 0; i < height; i++) {
			for (j = 0; j < left + 2; j++) {
				printf(" ");
			}
			printf("|\n");
		}
		isFire = 0;
		if (barY == left + 2) {
			isKilled = 1;
		}
	}

	//�ɻ�ͼ��
	for (j = 0; j < left; j++) {
		printf(" ");
	}
	printf("  *\n");
	for (j = 0; j < left; j++) {
		printf(" ");
	}
	printf("*****\n");
	for (j = 0; j < left; j++) {
		printf(" ");
	}
	printf(" i i\n");
}

int main()
{
	srand((unsigned)time(NULL));
	while (1) {
		updateAction();
		updateUser();
		show();
	}
}