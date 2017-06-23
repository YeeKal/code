/*graph with adjacency matrix*/
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

void printGraph(link *ps, int v);
void insertNode(link p, int n);
void insertConnect(link *ps, int m, int n);
void freeGraph(link *ps, int v);
void freeList(link p);

int main() {
	int v, m, n;
	puts("Enter the number of vertices:");
	scanf("%d", &v);
	link *ps;
	//memory allocation
	ps = (link *)malloc(v*sizeof(link));
	if (ps == NULL) {
		puts("Mempry allocation failed.");
		exit(1);
	}
	//initialize
	for (int i = 0; i < v; i++) {
		ps[i] = NULL;
	}
	//connect
	while (scanf("%d %d", &m, &n) == 2) {
		insertConnect(ps, m, n);
		printGraph(ps, v);
	}
	freeGraph(ps, v);
	return 0;
}
void insertNode(link *ptr, int n) {
	link p = *ptr, head = p;
	while (p != NULL) {
		head = p;
		p = p->next;
	}
	p = (link)malloc(sizeof(struct node));
	p->item.num = n;
	p->next = NULL;
	if (head == NULL)
		*ptr = p;
	else
		head->next = p;
}
void insertConnect(link *ps, int m, int n) {
	if (m != n) {
		insertNode(&ps[m], n);
		insertNode(&ps[n], m);
	}
}

void printGraph(link *ps, int v) {
	link t;
	for (int i = 0; i < v; i++) {
		printf("%-2d|", i);
		t = ps[i];
		while (t != NULL) {
			printf("%4d", t->item.num);
			t = t->next;
		}
		printf("\n");
	}
}
void freeGraph(link *ps, int v) {
	for (int i = 0; i < v; i++) {
		freeList(ps[i]);
	}
}
void freeList(link p) {
	link r;
	while (p != NULL) {
		r = p->next;
		free(p);
		p = r;
	}
}