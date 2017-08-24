/*pushdown stack with linked list*/
#include<stdio.h>
#include<string.h>
#include"Item.h"


typedef int Item;
typedef struct node* link;
struct node {
	Item item;
	link next;
};

static link head;

link new(Item item, link head){
	link x = (link)malloc(sizeof(link));
	x->next = head;
	x->item = item;
	return x;
}
void StackInit() {
	head = NULL;
}
int StackEmpty() {
	return head == NULL;
}
void StackPush(Item item) {
	head = new(item, head);
}
Item StackPop(void) {
	Item item = head->item;
	link x = head->next;
	free(head);
	head = x;
	return item;
}
int main(int argc, char *argv[]) {
	char *a = argv[1];
	int i, n = strlen(a);
	StackInit();
	for (i = 0; i < n; i++) {
		if (a[i] == '+')
			StackPush(StackPop() + StackPop());
		if(a[i]=='*')
			StackPush(StackPop()*StackPop());
		if ((a[i] >= '0') && (a[i] <= '9'))
			StackPush(a[i]);
	}
	printf("result:%d", StackPop());
	return 0;
}