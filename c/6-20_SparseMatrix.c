#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>



//store value in sparse matrix
typedef struct {
	int col;
	int row;
	int val;
}matr;

int Horner(int *list, int start, int num, int x);
void transmatr(matr a[], matr b[]);
void putmatr(matr a[]);
void fast_transmatr(matr a[], matr b[]);
void multmatr(matr a[], matr b[], matr *c);

int main() {
	matr a[9] = { {6,6,8},{0,0,15},{0,3,22},{0,5,-15},{1,1,11},{1,2,3},{2,3,-6},{4,0,91},{5,2,28} };
	matr b[9];
	matr c[18];
	
	transmatr(a, b);
	puts("the first matrix:");
	putmatr(a);
	puts("After transpoted:");
	putmatr(b);
	multmatr(a, b, c);
	puts("a * b is:");
	putmatr(c);

	return 0;
}
//output matrix
void putmatr(matr a[]) {
	int n, i, j,t,c1=0,c2=0,r1=-1,r2=0;
	int coln = 0;
	int rown = 0;
	int k = 0;
	coln = a[0].col;
	rown = a[0].row;
	n = a[0].val;
	if (n == 0)
		exit(1);
	for (i = 1; i <= n; i++) {
		//col
		c2= a[i].col - c1;
		if (c2 > 0) {/*补充空格和空行*/
			for (t = 0; t < c2; t++) {
				if (r1 < rown-1) {
					for (j = 0; j < rown-1 - r1; j++) {
						printf("%6d", k);
					}
				}
				printf("\n");
				r1 = -1;
			}
			
		}
		/**/
		c1= a[i].col;
		//row
		r2 = a[i].row - r1-1;
		for (j = 0; j < r2; j++)
			printf("%6d",k);
		r1 = a[i].row;
		//val
		printf("%6d", a[i].val);
	}
	c2 = coln - c1;	/*补充最后一个元素后面的空格和空行*/
	if (c2 > 0) {
		for (t = 0; t < c2; t++) {
			if (r1 < rown-1) {
				for (j = 0; j < rown-1 - r1; j++) {
					printf("%6d", k);
				}
			}
			printf("\n");
			r1 = -1;
		}
	}
	putchar('\n');
}
//基于ADT matr的稀疏矩阵转置
//单个元素转置
void transmatr(matr a[], matr b[]) {
	int n, i, j, num=1;
	n = a[0].val;
	b[0].val = n;
	b[0].col = a[0].col;
	b[0].row = a[0].row;
	for (i = 0; i < a[0].row; i++) {
		for (j = 1; j <=n; j++) {
			if (a[j].row== i) {
				(*(b+num)).row = a[j].col;
				(*(b + num)).col = a[j].row;
				(*(b + num)).val = a[j].val;
				num++;
			}
		}
	}

}
/*快速转置，基于一行/一列*/
void fast_transmatr(matr a[], matr b[]) {
	
}
/*矩阵乘法diy*/
void multmatr(matr a[], matr b[], matr *c) {
	int cterms=0;
	int addele = 0;
	int aterms, bterms;
	aterms = a[0].val;
	bterms = b[0].val;
	(*(c+0)).col = a[0].col;
	(*(c + 0)).row=b[0].row;
	for (int j = 0; j < a[0].col; j++) {
		for (int p = 0; p < b[0].row; p++) {
			for (int i = 1; i <= aterms; i++) {
				if (a[i].col == j) {//a element
					for (int k = 1; k <= bterms; k++) {
						if (b[k].row == p) {//one b row
							if (a[i].row == b[k].col) {//b element
								addele = addele + a[i].val*b[k].val;
							}
						}
					}
				}
			}
			if (addele != 0) {
				cterms++;
				(*(c + cterms)).col = j;
				(*(c + cterms)).row = p;
				(*(c + cterms)).val = addele;
				addele = 0;
			}
		}
	}
	printf("the citems:%d\n", cterms);
	(*(c + 0)).val = cterms;
}