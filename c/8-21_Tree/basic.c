/*tree*/
#include<stdio.h>
#include<stdlib.h>
#include"Item.h"
#include"function.h"


int main() {
	link head=NULL;
	link temp=NULL;
	//1
	temp = (link)malloc(sizeof(struct node));
	temp->item.num = 1;
	temp->next1 = temp->next2 = NULL;
	head = temp;
	//2
	temp = (link)malloc(sizeof(struct node));
	temp->item.num = 2;
	temp->next1 = temp->next2 = NULL;
	head->next1 = temp;
	//3
	temp = (link)malloc(sizeof(struct node));
	temp->item.num = 3;
	temp->next1 = temp->next2 = NULL;
	head->next2=temp;
	//4
	temp = (link)malloc(sizeof(struct node));
	temp->item.num = 4;
	temp->next1 = temp->next2 = NULL;
	head->next1->next1=temp;
	//7
	temp = (link)malloc(sizeof(struct node));
	temp->item.num = 7;
	temp->next1 = temp->next2 = NULL;
	head->next1->next1->next1 = temp;
	//5
	temp = (link)malloc(sizeof(struct node));
	temp->item.num = 5;
	temp->next1 = temp->next2 = NULL;
	head->next2->next1 = temp;
	//6
	temp = (link)malloc(sizeof(struct node));
	temp->item.num = 6;
	temp->next1 = temp->next2 = NULL;
	head->next2->next2 = temp;

	puts("preorder:");
	preTraverse(head);
	putchar('\n');

	puts("inorder:");
	inTraverse(head);
	putchar('\n');

	puts("preorder not by recursion");
	stackTraverse(head);
	putchar('\n');

	puts("layorder not by recursion");
	layTraverse(head);
	putchar('\n');

	printf("Nodes: %d\n", countNode(head));
	printf("height: %d\n", countHeight(head));

	puts("Show tree drawing:");
	showTree(head, 1);
	return 0;
}
