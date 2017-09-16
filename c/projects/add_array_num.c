#include<stdio.h>
int sum(int *start, int *end);
#define SIZE 10
int main()
{
	int b[SIZE] = { 1,2,3,4,5,6,7,8,9,10 };
	printf("the sum of members in b is %d\n", sum(b, b + SIZE));
}
int sum(int *start, int *end)
{
	int total = 0;
	while (start < end)
		/*
		end所指向在最后一个元素之后，c保证在为数组元素分配空间时
    指向数组之后的第一个位置的指针合法，
		这种“越界”指针可使函数调用形式更加整洁
		*/
	{
		total = total + *start;
		start++;
	}
	return total;
}
