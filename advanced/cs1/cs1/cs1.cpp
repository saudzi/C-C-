// cs1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma warning (disable : 4996)
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "Date.h"
#include <time.h>
#include <random>
#include <fstream>
#include "string"
#include <map>
#include <list>
#include <initializer_list>



using namespace std;


Date::Date()
{
	time(&Now);
	struct tm Tm;
	localtime_s(&Tm, &Now);
	Day = Tm.tm_mday; // 1...31
	strcpy_s(Month, MonthNames[Tm.tm_mon]); // 0...11
	iMonth = Tm.tm_mon + 1;
	Year = Tm.tm_year + 1900; // current year - 1900
}
Date::Date(int d, int m, int y)
{
	SetYear(y);
	SetMonth(m);
	SetDay(d);
}
Date::Date(const Date& Original)
{ // copy constructor
	Day = Original.Day;
	Year = Original.Year;
	iMonth = Original.iMonth;
	strcpy_s(Month, Original.Month);
}
Date::~Date()
{
	if (pText)
		delete pText;
}
bool Date::IsLeap(int y)
{ // from https://www.programiz.com/c-programming/examples/leap-year
	if (y % 4 == 0)
	{
		if (y % 100 == 0)
		{	// year is divisible by 400, hence the year is a leap year
			if (y % 400 == 0)
				return true;
			else
				return false;
		}
		else
			return true;
	}
	else
		return false;
}
char* Date::ToString() const
{
	(const_cast<Date*>(this))->pText = new char[12];
	sprintf_s(pText, 12, "%02d %s %d", Day, Month, Year);
	return pText;
}
void Date::SetMonth(int m)
{
	if (m < 1 || m > 12)
		throw exception("Wrong month");
	strcpy_s(Month, MonthNames[m - 1]);
	iMonth = m;
}
int Date::GetMonth(char* pBuf, int nBuf) const
{
	if (!pBuf || nBuf < 4)
		return iMonth;
	strcpy_s(pBuf, nBuf, Month);
	return iMonth;
}
void Date::SetYear(int y)
{
	if (y < 0)
		throw exception("Wrong year");
	Year = y;
}
void Date::SetDay(int d)
{
	if (d < 1 || d > 31)
		throw exception("Wrong day");
	if ((iMonth == 4 || iMonth == 6 || iMonth == 9 || iMonth == 11) && d == 31)
		throw exception("Wrong day");
	if (iMonth == 2)
	{
		if (IsLeap(Year))
		{
			if (d > 29)
				throw exception("Wrong day");
		}
		else
		{
			if (d > 28)
				throw exception("Wrong day");
		}
	}
	Day = d;
}
void Date::SetDate(int d, int m, int y)
{
	SetYear(y);
	SetMonth(m);
	SetDay(d);
}
bool Date::operator==(const Date& other) const
{
	if (Day == other.Day && iMonth == other.iMonth && Year == other.Year)
		return true;
	else
		return false;
}
bool Date::operator<(const Date& other) const
{
	if (Year != other.Year)
		return Year < other.Year;
	if (iMonth != other.iMonth)
		return iMonth < other.iMonth;
	return Day < other.Day;
}
Date& Date::operator=(const Date& Right)
{
	//cout << "operator= called\n";
	if (this == &Right) // & - address operator
		return *this; // necessary for expressions like d1 = *pd where pd points to d1
	Day = Right.Day;
	Year = Right.Year;
	iMonth = Right.iMonth;
	strcpy_s(Month, Right.Month);
	return *this;
}
Date Date::CreateRandomDate(Date begin, Date end)
{   // returns a date between begin and end
	// https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
	tm tm_begin, tm_end;
	memset(&tm_begin, 0, sizeof tm_begin);
	memset(&tm_end, 0, sizeof tm_end);
	tm_begin.tm_mday = begin.GetDay();
	tm_end.tm_mday = end.GetDay();
	tm_begin.tm_mon = begin.GetMonth() - 1;
	tm_end.tm_mon = end.GetMonth() - 1;
	tm_begin.tm_year = begin.GetYear() - 1900;
	tm_end.tm_year = end.GetYear() - 1900;
	time_t lower = mktime(&tm_begin);
	time_t upper = mktime(&tm_end);
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr((unsigned int)lower, (unsigned int)upper);
	time_t time_t_rand = distr(eng);
	tm tm_rand;
	localtime_s(&tm_rand, &time_t_rand);
	return Date(tm_rand.tm_mday, tm_rand.tm_mon + 1, tm_rand.tm_year + 1900);
}

default_random_engine gela(time(0));

class Item
{
private:
	char Group; // Any from range 'A'...'Z'
	int Subgroup; // Any from range 0...99
	string Name; // Any, but not empty
	Date Timestamp; // Any
	
public:
	Item() {
		fstream file;
		file.open("Birds.txt", fstream::in);

		if (file.good() == 0) {
			printf("\nNot good! - gcount: %d\n", file.gcount());
			throw;
		}

		if (!file) {
			cout << endl << "Cannot open file!" << endl;
			throw;
		}

		int taken = 0;
		char newl = '\n';
		char period = '.';
		char end = '\0';

		file.seekg(ios_base::beg);

		vector<string> birds;

		while (1) { // 
			char * lime = (char*)malloc(100);


			file.getline(lime, 50, newl);


			//cout <<endl<<file.good() << file.eof() << file.fail() << file.bad() << file.rdstate()<<endl;


			if (!lime) {
				printf("\n%d items taken\n", taken);
				break;
			}

			if (!file.good()) {
				//printf("\n645: file not good, %d items copied\n", taken);
				break;
			}

			string line(lime);
			birds.push_back(line);

			taken++;
			//cout<<endl<<this->GetItemsNumber();

			if (file.good() == 0) {
				printf("\n693: file not good, %d items copied\n", taken);
				break;
			}
		} 

		/////////////////////////////////
		

		Name = birds[gela() % birds.size()];
		Group = (char)(65 + gela() % 26);
		Subgroup = gela() % 100;
		Date startdate = Date(30,5,1980);
		Date enddate = Date(30, 5, 2020);
		Timestamp = Date::CreateRandomDate(startdate,enddate);
		
	} // Fills the four fields above with preudo-random values

	char getgroup() { return Group; }
	int getsub() { return Subgroup; }
	string getname() { return Name; }
	Date getdate() { return Timestamp; }

	
	void showall() {
		cout << Group << endl << Subgroup << endl << Name << endl << Timestamp.ToString() << endl;
	}
	
	

	Item(char group, int sgroup, string name, Date date) {
		Group = group;
		Subgroup = sgroup;
		Name = name;
		Timestamp = date;
	}
	Item(const Item & from) {
		Group = from.Group;
		Subgroup = from.Subgroup;
		Name = from.Name;
		Timestamp = from.Timestamp;
	} // copy constructor
	~Item() {
	}
	
};

class Data {
private:
	map<char, map<int, list<Item *> *> *> DataStructure;
	

public:
	int itemsadded = 0;
	Data(int n) {
		
		for (int i = 0; i < n; i++) {
			Item* item = new Item();

			if (DataStructure.find(item->getgroup()) == DataStructure.end()) {
				DataStructure[item->getgroup()] = new map<int, list<Item *>*>;
			}
			auto p2map = DataStructure[item->getgroup()];

			if (p2map->find(item->getsub()) == p2map->end() ) {
				(*p2map)[item->getsub()] = new list<Item *>;	
			}
			auto p2list = (*p2map)[item->getsub()];

			bool notinset = 1;
			
			for (list<Item *>::iterator itt = p2list->begin(); itt !=p2list->end(); itt++) {

				if ((*itt)->getname() == item->getname()) {
					notinset = 0;
				}
			}
			if (notinset) {
				p2list->push_back(item);
				cout << "item added" << endl;
				itemsadded++;
			}
			else { i--; }
		}
	}
	Data();


	void Printall() {
		for (map<char, map<int, list<Item *> *> *>::iterator it = DataStructure.begin(); it != DataStructure.end(); ++it) {
			cout << it->first<<endl;
			for (map<int, list<Item *>*>::iterator it2 = DataStructure[it->first]->begin(); it2 != DataStructure[it->first]->end(); ++it2) {
				cout << "\t" << it2->first<<"___________"<<endl;
				for (list<Item *>::iterator it3 = it2->second->begin(); it3 != it2->second->end(); it3++) {
					
					cout <<"\t\t--- "<<(*it3)->getname() <<endl;
				}
			}
			
		}
	}

	~Data();

};

int main()
{
    std::cout << "Hello World!\n"; 
	
	Item* nice = new Item;
	Data* notnice = new Data(200);
	notnice->Printall();
	//nice->showall();
}
