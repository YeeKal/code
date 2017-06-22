#include<stdio.h>
#include<stdlib.h>
void printNum(int nums[], int n);
int main(){
	int *nums;
	int n;
	printf("Enter the number:");
	scanf("%d", &n);
	nums = (int *)malloc(n*sizeof(int));
	printNum(nums,n);
	free(nums);
	return 0;
}
//find prime number and print
void printNum(int nums[],int n) {
	for (int i = 0; i < n; i++)
		nums[i] = 1;
	for (int i = 2; i <= n; i++) {
		if (nums[i-1]) {
			for (int j = i; j*i <= n; j++) {
				nums[i*j-1] = 0;
			}
		}
	}
	for (int i = 0; i < n; i++)
		if (nums[i]) printf("%d\n", i + 1);
}