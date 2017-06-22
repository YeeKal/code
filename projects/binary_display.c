/*display binary of an interger*/
#include<stdio.h>
char * itobs(int, char *);
void show(const char *);

int main() {
	char bitstr[8 * sizeof(int)+1];
	int num;

	puts("enter an interger and see them in binary.");
	while (scanf("%d", &num) == 1) {
		itobs(num, bitstr);
		printf("%d is ", num);
		show(bitstr);
		getchar();
		putchar('\n');
	}
	puts("bye");
	return 0;	
}

char * itobs(int n, char *ps) {
	int i;
	static int size = 8 * sizeof(int);

	for (i = size - 1; i >= 0; i--, n >>= 1)
		ps[i] = (01 & n) + '0';
	ps[size] = '\0';
	return ps;
}

void show(const char *str) {
	int i = 0;
	while (str[i]) {
		putchar(str[i]);
		if (++i % 4 == 0 && str[i])
			putchar(' ');
	}
}