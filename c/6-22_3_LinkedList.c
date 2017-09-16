#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct {
	int num;
}Item;
typedef struct node* link;
struct node {
	Item item;
	link next;
};
link reverseList(link l1);
link sortedList(link l1);
void printList(link head);
int main() {
	int peopleNum;
	int i;
	link head = NULL, pre = NULL, cur;

	printf("Enter number of people and circuler number:");
	scanf("%d", &peopleNum);
	srand(time(NULL));

	//create list
	for (i = 1; i <= peopleNum; i++) {
		cur= (link)malloc(sizeof(struct node));
		cur->item.num = (rand()%100+1);
		cur->next = NULL;
		if (head == NULL) {
			head = cur;
			pre = cur;
		}
		else {
			pre ->next = cur;
			pre = cur;
		}
	}
	puts("Before:");
	printList(head);

	head = reverseList(head);
	puts("After:");
	printList(head);

	head = sortedList(head);
	puts("After sorted:");
	printList(head);

	return 0;
}

void printList(link head) {
	link cur;
	for (cur = head; cur != NULL; cur = cur->next)
		printf("the num:%d\n", cur->item.num);
}
link reverseList(link l1) {
	link t,head=NULL,r;
	for (t = l1; t != NULL; t = r) {
		r = t->next;
		t->next = head;
		head = t;
	}
	return head;
}
//rearrange items
link sortedList(link l1) {
	link t,k;
	int n;
	for (t = l1; t->next != NULL; t = t->next) {
		for (k = t->next; k != NULL; k = k->next) {
			if (t->item.num < k->item.num) {
				n = t->item.num;
				t->item.num = k->item.num;
				k->item.num = n;
			}
		}
	}
	return l1;
}