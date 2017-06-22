///*choose leader from n people about m  according inputs*/
//#include<stdio.h>
//#include<stdlib.h>
//
//typedef struct {
//	int num;
//}Item;
//typedef struct node* link;
//struct node {
//	Item item;
//	link next;
//};
//void deleteNodeK(link *head, int peopleNum, int deNum,int k);
//int main() {
//	int peopleNum,peopleRe;
//	int circularNum;
//	int i;
//	link head=NULL,pre=NULL,cur;
//
//	printf("Enter number of people and circuler number:");
//	scanf("%d%d", &peopleNum, &circularNum);
//	if (peopleNum < 1 || circularNum < 1) {
//		puts("invalid numbers");
//		exit(1);
//	}
//	//create circular
//	for (i = 1; i <= peopleNum; i++) {
//		if (i == 1) {
//			head = (link)malloc(sizeof(struct node));
//			head->item.num = i;
//			head->next = NULL;
//			pre = head;
//		}
//		else {
//			cur= (link)malloc(sizeof(struct node));
//			cur->item.num = i;
//			cur->next = NULL;
//			pre->next = cur;
//			pre = cur;
//		}
//		pre->next = head;
//	}
//	//choose leader and delete people
//	int surplus;
//	link *hptr;
//	hptr = &head;
//	peopleRe = peopleNum;
//	for (i = 0; i < peopleNum - 1; i++) {
//		if (peopleRe > 1) {
//			surplus = circularNum%peopleRe;
//			if (surplus == 0) {
//				deleteNodeK(hptr, peopleRe,peopleRe,i+1);
//			}
//			else
//				deleteNodeK(hptr, peopleRe, surplus,i+1);
//			peopleRe--;
//		}
//	}
//	printf("The leader number is:%d\n", head->item.num);
//	return 0;
//}
////delete node in k
//void deleteNodeK(link *head,int peopleRe,int deNum,int k) {
//	link cur=NULL,t;
//	if (deNum == 1) {
//		for (int i = 0; i < peopleRe; i++) {
//			if (cur == NULL) {
//				cur = *head;
//			}
//			else
//				cur = cur->next;
//		}
//	}
//	else {
//		for (int i = 0; i < deNum-1; i++) {
//			if (cur == NULL)
//				cur = *head;
//			else
//				cur = cur->next;
//		}
//	}
//	t = cur->next;
//	printf("%d times:%d\n",k, t->item.num);
//	cur ->next =*head= t->next;
//	free(t);
//}