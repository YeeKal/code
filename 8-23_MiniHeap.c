/*heap--min heap*/
#include<stdio.h>
#include<stdlib.h>

void createHeap(int *heap);
void insertHeap(int *heap, int item);
void percolateUp(int *heap);
void swap(int *p, int s, int e);
int emptyHeap(int *heap);
void percolateDown(int *heap);
int deleteHeap(int *heap);
int main(void)
{
	int *heap;
	int a[9] = { 1,2,5,3,9,6,8,10,7 };
	int val;
	heap = (int *)malloc(sizeof(int) * 20);
	for (int i = 1; i <=9; i++)
		heap[i] = a[i-1];
	heap[0] = 9;
	puts("Test:");
	val = deleteHeap(heap);
	printf("The deleted num is: %d\n", val);
	printf("input a num: (q to quit)");
	while (scanf("%d", &val) == 1) {
		insertHeap(heap, val);
		printf("now the item num  is:%d\n", heap[0]);
		for (int i = 1; i <= heap[0]; i++) {
			printf("%5d", heap[i]);
			if (i % 5 == 0)
				putchar('\n');
		}
		putchar('\n');
		printf("input a num:(q to quit) ");
	}
	
	
	val = deleteHeap(heap);
	printf("The deleted num is: %d\n", val);
	puts("Output heap:");
	for (int i = 1; i <= heap[0]; i++) {
		printf("%5d", heap[i]);
		if (i % 5 == 0)
			putchar('\n');
	}
	putchar('\n');
	free(heap);
	return 0;
}
void createHeap(int *heap) {

}
void insertHeap(int *heap, int item) {
	heap[0]++;
	heap[heap[0] ] = item;
	percolateUp(heap);
}

void percolateUp(int *heap) {
	int index_child = heap[0];
	int index_father;
	while (index_child > 1) {
		index_father = index_child / 2;
		if (heap[index_child] < heap[index_father])
			swap(heap, index_child, index_father);
		else
			break;
		index_child = index_father;
	}
}
void swap(int *p, int s, int e) {
	int num = p[s];
	p[s] = p[e];
	p[e] = num;
}
int deleteHeap(int *heap) {
	if (emptyHeap(heap)) {
		puts("heap is empty.invalid operation!");
		exit(1);
	}
	int val = heap[1];
	swap(heap, 1, heap[0]);
	//heap[heap[0]] = NULL;
	heap[0]--;
	percolateDown(heap);
	return val;
}
int emptyHeap(int *heap) {
	if (heap[0] < 1)
		return 1;
	else
		return 0;
}
void percolateDown(int *heap) {
	int index_father = 1;
	int index_child=2*index_father;
	int index_child2 = index_child + 1;
	int index_mid;
	while (1) {
		if (index_child > heap[0])
			return;
		else if (index_child2 > heap[0])
			index_mid = index_child;
		else {
			if (heap[index_child] < heap[index_child2])
				index_mid = index_child;
			else
				index_mid = index_child2;
		}
		if (heap[index_mid] < heap[index_father])
			swap(heap, index_mid, index_father);
		else
			return;
		index_father = index_mid;
		index_child = 2 * index_father;
		index_child2 = index_child + 1;
	}
}