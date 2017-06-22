#include<stdio.h>
#include<stdlib.h>

typedef struct {
	int num;
}Item;
typedef struct node* link;
struct node {
	Item item;
	link next;
};
int main() {
	int peopleNum;
	int circularNum;
	int i;
	link head = NULL, pre = NULL, cur;

	printf("Enter number of people and circuler number:");
	scanf("%d%d", &peopleNum, &circularNum);
	if (peopleNum < 1 || circularNum < 1) {
		puts("invalid numbers");
		exit(1);
	}
	//create circular
	head = (link)malloc(sizeof(struct node));
	head->item.num = 1;
	head->next = head;
	cur = head;
	for (i = 2; i <= peopleNum; i++) {
		pre = (link)malloc(sizeof(struct node));
		cur->next=pre;
		pre->item.num = i;
		pre->next = head;
		cur = pre;
	}
	//delete
	while (cur->next != cur) {
		for (i = 0; i < circularNum-1; i++)
			cur = cur->next;
		pre = cur->next;
		printf("deleted people:%d\n", pre->item.num);
		cur->next = pre->next;
		free(pre);
	}
	printf("The leder:%d\n", cur->item.num);
	return 0;
}