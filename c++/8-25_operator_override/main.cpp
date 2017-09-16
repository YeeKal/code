//main.cpp -- using Time class
#include<iostream>
#include "mytime0.h"
using namespace std;

int main(void)
{
	Time planning;
	Time coding(2, 40);
	Time fixing(5, 55);
	Time total;

	cout << "planning time=";
	planning.Show();
	cout << endl;

	cout << "coding time=";
	coding.Show();
	cout << endl;

	cout << "fixing time=";
	fixing.Show();
	cout << endl;

	total = coding.operator + (fixing);
	cout << "override operator + \n total time=";
	total.Show();
	cout << endl;

	total = coding+fixing;
	cout << "override operator + \n total time="<<total;//override <<
	cout << endl;
	return 0;
}

