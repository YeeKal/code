#include<stdio.h> /*standard input&output*/
/*
Author:Yee
Description:print year calendar within 2001-2099
Last Update:2017/4/11
*/
void Print_Month(int start_day, int day_length);

void main()
{
	int last_day_2000 = 7,last_day_lastmonth,j,i;
	int year, year_length,leep_year_num,not_leep_left;
	int month_length[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	//enum Weekday {Mon,Tue,Wed,Thu,Fri,Sat,Sun};
	//enum Weekday week;
	char *week[] = { "Mon","Tue","Wed","Thu","Fri","Sat","Sun" };
	char *month[] = { "January","Fabruary","March","April","May","June",
					"July","August","September","October","November","December" };

	printf("Please input year within 2001-2099:");
	scanf("%d", &year);
	leep_year_num = (year - 2001) / 4;
	not_leep_left = (year - 2001) % 4;
	last_day_lastmonth = (leep_year_num * 1461 + not_leep_left * 365 + last_day_2000) % 7;
	if (not_leep_left == 3)
	{
		month_length[1]++;
	}

	printf("the year:%d\n", year);

	for (j = 1; j <= 12; j++)
	{
		printf("%s\n", month[j - 1]);
		for (i=0; i< 7;i++)
		{
			printf("%6s", week[i]);
		}

		printf("\n");

		Print_Month(last_day_lastmonth, month_length[j - 1]);
		last_day_lastmonth = (last_day_lastmonth + month_length[j - 1]) % 7;
		printf("\n\n");
	}

	printf("print enter to quit...\n");
  getchar();
	getchar();
}

void Print_Month(int last_day_lastmonth, int month_length)
{
	int i;
	for (i = 1; i <= last_day_lastmonth + month_length; i++)
	{
		if (i <= last_day_lastmonth)
		{
			printf("      ");
		}
		else
		{
			printf("%6d", i-last_day_lastmonth);
		}

		if (i % 7 == 0)
		{
			printf("\n");
		}
	}
}
