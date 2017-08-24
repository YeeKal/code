/*struct and rudimentary function*/

typedef struct {
	int num;
}Item;
typedef struct node* link;
struct node {
	Item item;
	link next1, next2;
};

//for stack
typedef link Items;
typedef struct nodes* links;
struct nodes {
	Items items;
	links next;
};


links new(Items item, links head){
	links x = (links)malloc(sizeof(links));
	x->next = head;
	x->items = item;
	return x;
}

//栈-后进先出
//队列：先进先出
//以下三个函数中，出栈用同一个，上层出
//入栈不同，栈是上层入，队列是下层
void StackPush(Items item,links *head) {
	//上层加元素
	*head= new(item, *head);//pointer operation
}
Items StackPop(links *head) {
	Items item = (*head)->items;
	links x = (*head)->next;
	//free(*head);
	*head = x;
	return item;
}
void QuenePush(Items item, links *head) {
	//在底层加元素
	//难点：如若不通过指针很难实现
	links x = (links)malloc(sizeof(links));
	x->items = item;
	x->next = NULL;
	links t = *head;
	if (t == NULL)
		*head = x;
	else {
		while (t->next!= NULL) {
			t = t->next;
		}
		t->next = x;//t->next 是一个已被链接的指针，若换成t则出错
	}
}
/*print single node according its height*/
void printSingleNode(int name, int h) {
	for (int i = 0; i < h; i++)
		printf("    ");
	printf("%d\n", name);
}