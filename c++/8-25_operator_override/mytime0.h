//mytime0 --Time class before operator overloading
#ifndef MYTTIME0_H_
#define MYTIME0_H_
#include<iostream>
class Time {
private:
	int hours;
	int minutes;
public:
	Time();
	Time(int h, int m = 0);
	void AddMin(int m);
	void AddHr(int h);
	void Reset(int h = 0, int m = 0);
	Time Sum(const Time &t)const;
	Time operator+ (const Time & t) const;
	void Show()const;
	friend std::ostream & operator<< (std::ostream & os, const Time &t);
};
#endif
