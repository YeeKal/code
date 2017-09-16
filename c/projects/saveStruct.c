/*structLen.c	*/
#include<stdio.h>
#include<stdlib.h>
#define MAXTITL 40
#define MAXAUTL 40
#define MAXBKS 10
struct book {
	char title[MAXTITL];
	char author[MAXAUTL];
	float value;
};	//struct definition end and 注意双引号

int main() {
	struct book sjtu[MAXBKS];
	int count = 0;
	int index, filecount;
	FILE *pb;
	int size = sizeof(struct book);

	if ((pb = fopen("book.dat", "a+b")) == NULL) {
		fputs("can't open book.dat.\n", stderr);
		exit(1);
	}
	rewind(pb);
	while (count < MAXBKS && fread(&sjtu[count], size, 1, pb) == 1) {
		if (count == 0) {
			puts("Current contents of book.dat:");
		}
		printf("%s by %s: $%.2f\n", sjtu[count].title,
			sjtu[count].author, sjtu[count].value);
		count++;
	}
	filecount = count;
	if (count == MAXBKS) {
		fputs("the book.dat file if full.", stderr);
		exit(2);
	}
	puts("please add new book title:");
	puts("please [enter] at the start of a line to stop.");
	while (count < MAXBKS && gets(sjtu[count].title) != NULL && sjtu[count].title[0] != '\0') {
		puts("now enter the author:");
		gets(sjtu[count].author);
		puts("now enter the value");
		scanf("%f", &sjtu[count++].value);
		while (getchar() != '\n')
			continue;  //clear  imput line
		if (count < MAXBKS)
			puts("Enter next title:");
	}

	if (count > 0) {
		puts("here is the list of your books:");
		for (index = 0; index < count; index++)
			printf("%s by %s: $%.2f\n", sjtu[index].title,
				sjtu[index].author, sjtu[index].value);
		fwrite(&sjtu[filecount], size, count - filecount, pb);//写入多个，是开始地址位
	}
	else
		puts("no books.\n");
	puts("bye.\n");
	fclose(pb);

	return 0;
}