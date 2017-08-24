/*second layer function*/

void showTree(link head,int h) {
	if (head == NULL)
		return;
	showTree(head->next1,h+1);
	printSingleNode(head->item.num, h);
	showTree(head->next2, h + 1);
}
/*count nodes in tree*/
int countNode(link head) {
	if (head == NULL)
		return 0;
	else
		return countNode(head->next1) + countNode(head->next2) + 1;
}
/*count height in tree*/
int countHeight(link head) {
	int u, v;
	if (head == NULL)
		return 0;
	u = countHeight(head->next1);
	v = countHeight(head->next2);
	if (u > v)
		return u + 1;
	else
		return v + 1;
}
/*preorder by recursion*/
void preTraverse(link head) {
	if (head != NULL) {
		printf("%5d", head->item.num);
		preTraverse(head->next1);
		preTraverse(head->next2);
	}
}
/*inorder by recursion*/
void inTraverse(link head) {
	if (head != NULL) {
		inTraverse(head->next1);
		printf("%5d", head->item.num);
		inTraverse(head->next2);
	}
}
/*postorder
they have the same statements
just rearrange them*/

/*preorder not by recursion*/
void stackTraverse(link head) {
	link s = head;
	links st = NULL;
	StackPush(s, &st);
	while (st != NULL) {
		s = StackPop(&st);
		printf("%5d", s->item.num);
		if (s->next2 != NULL)
			StackPush(s->next2, &st);
		if (s->next1 != NULL)
			StackPush(s->next1, &st);
	}
}

/*layer order*/
void layTraverse(link head) {
	link s = head;
	links st = NULL;
	QuenePush(s, &st);
	while (st != NULL) {
		s = StackPop(&st);
		printf("%5d", s->item.num);
		if (s->next1 != NULL)
			QuenePush(s->next1, &st);
		if (s->next2 != NULL)
			QuenePush(s->next2, &st);
	}
}
