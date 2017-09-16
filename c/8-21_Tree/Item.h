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

//ջ-����ȳ�
//���У��Ƚ��ȳ�
//�������������У���ջ��ͬһ�����ϲ��
//��ջ��ͬ��ջ���ϲ��룬�������²�
void StackPush(Items item,links *head) {
	//�ϲ��Ԫ��
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
	//�ڵײ��Ԫ��
	//�ѵ㣺������ͨ��ָ�����ʵ��
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
		t->next = x;//t->next ��һ���ѱ����ӵ�ָ�룬������t�����
	}
}
/*print single node according its height*/
void printSingleNode(int name, int h) {
	for (int i = 0; i < h; i++)
		printf("    ");
	printf("%d\n", name);
}