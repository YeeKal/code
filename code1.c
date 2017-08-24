/*hanoi*/
void hanoi(int n,char a,char b,char c) {
	if (n == 1){
		printf("%c -> %c\n", a, c);
	}
	else {
		hanoi(n - 1, a, c, b);
		printf("%c -> %c\n", a, c);
		hanoi(n - 1, b, a, c);
	}
}

/*
derive max number with divide and conquer 
*/
int maxNum(int a[], int i, int e) {
	int m1, m2;
	if (i == e)
		return a[i-1];  //kill condition
	int m = (i + e) / 2;
	m1 = maxNum(a, i, m);
	m2 = maxNum(a, m + 1, e);
	if (m1 >= m2)
		return m1;
	else
		return m2;
}
/*
input:two integers
output:max common factor
*/
int maxFactor(int x, int y) {
	if (y==0)
		return x;
	else
		return maxFactor(y, x%y);
}