#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void swap(int numP[], int i, int j);
void selectSort(int *numP, int size);
void insertSort(int *numP, int size);
void shellSort(int *numP, int size);
void quickSort(int *numP, int s, int e);
int partion1(int *numP, int s, int e);
int partion2(int *numP, int s, int e);
void mergeArray(int *numP, int first, int mid, int last, int *temp);
void mergeSort(int *numP, int s, int e, int *temp);

int main(void) {
	int size;
	int *numP,*temp;
	time_t c_start, c_end;
	printf("How many nums would you want to be sorted: ");
	scanf("%d", &size);
	numP = (int *)malloc(sizeof(int)*size);
	temp = (int *)malloc(sizeof(int)*size);
	srand((unsigned)time(NULL));
	puts("nums generated displayed:");
	for (int i = 0; i < size; i++){
		*(numP + i) = (rand() % 1000);
		printf("%5d", numP[i]);
		if (((i + 1) % 20) == 0)
			putchar('\n');
	}
	putchar('\n');
	//shellSort(numP, size);
	//insertSort(numP, size);
	//selectSort(numP, size);
	//quickSort(numP, 0, size - 1);
	//
	c_start = clock();
	mergeSort(numP,0,size-1,temp);
	c_end = clock();

	puts("nums sorted displayed(s to big):");
	for (int i = 0; i < size; i++) {
		printf("%5d", numP[i]);
		if (((i + 1) % 20) == 0)
			putchar('\n');
	}	
	putchar('\n');
	printf("runningTime:%f\n", difftime(c_end, c_start));
	return 0;
}
void mergeArray(int *numP, int first, int mid, int last, int *temp) {
	int s=first,m = mid,e=mid+1;
	int k = 0;
	while (s <= m && e <= last) {
		if (numP[s] < numP[e])
			temp[k++] = numP[s++];
		else
			temp[k++] = numP[e++];
	}
	while (s <= m)
		temp[k++] = numP[s++];
	while (e <= last)
		temp[k++] = numP[e++];
	for (int i = 0; i < k; i++)
		numP[first + i] = temp[i];
}
void mergeSort(int *numP, int s, int e, int *temp) {
	if (s < e) {
		int m = (s + e) / 2;
		mergeSort(numP, s, m, temp);
		mergeSort(numP, m + 1, e, temp);
		mergeArray(numP, s, m, e, temp);
	}
}
void quickSort(int *numP, int s, int e) {
	int i;
	if (e > s) {
		i = partion2(numP, s, e);
		quickSort(numP, s, i - 1);
		quickSort(numP, i + 1, e);
	}
}
int partion1(int *numP, int s, int e) {
	//choose the referance num
	//ways to interchange two nums
	//two sides to find 
	int val;
	val = numP[e];
	int left=s-1, right=e;
	while (1) {
		while (numP[++left] < val);
		while (numP[--right] >= val)
			if (right == s)
				break;
		if (left >= right)
			break;
		swap(numP, left, right);
	}
	swap(numP, left, e);
	return left;
}
int partion2(int *numP, int s, int e) {
	//choose the referance num
	//ways to interchange two nums
	//one side to find 
	int val, num;
	val = numP[e];
	num = s;
	for (int i = s; i < e; i++) {
		if (numP[i] < val)
		{
			swap(numP, num, i);
			num++;
		}
	}
	swap(numP, num, e);
	return num;
}

void shellSort(int *numP, int size) {
	int row;
	for (row = 1; row <= (size - 1) / 9; row = 3 * row + 1);
	for (; row > 0; row /= 3) {
		for (int i = row; i < size; i++) {
			for (int j = i - row; j >= 0; j -= row) {
				if (numP[j] >numP[j + row])
					swap(numP, j, j+ row);
			}
			
		}
	}
}
void insertSort(int *numP, int size) {
	int i, j;
	for (i = 1; i < size; i++) {
		for (j = i - 1; j >= 0; j--) {
			if (numP[j] > numP[j + 1])
				swap(numP, j, j + 1);
		}
	}
}
void selectSort(int *numP, int size) {
	int i, j;
	for (i = 0; i < size-1; i++) {
		for (j = i + 1; j < size; j++) {
			if (numP[i] > numP[j])
				swap(numP, i, j);
		}
	}
}
void swap(int numP[], int i, int j) {
	int m;
	m = numP[i];
	numP[i] = numP[j];
	numP[j] = m;
}