#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
using namespace std;

class Student
{
	string id;
	string name;
	int group;
	set<string> doneTests;

public:
	Student() : id("0"), name("Нет имени"), group(0) {}
	Student(string id, string n, int g) {
		this->id = id;
		name = n;
		group = g;
	}
	string getId() const {
		return id;
	}
	string getName() const {
		return name;
	}
	int getGroup() const {
		return group;
	}
	const set<string>& getDoneTests() const {
		return doneTests;
	}
	int getCountDoneTests() const {
		return doneTests.size();
	}
	void setId(string id) {
		this->id = id;
	}
	void setName(string n) {
		name = n;
	}
	void setGroup(int g) {
		group = g;
	}
	void addDoneTest(string idTest) {
		doneTests.insert(idTest);
	}
	void addFullDoneTest(const set<string>& tests) {
		doneTests = tests;
	}
	void printStudent() const {
		cout << " | " << setw(6) << id << " | " << setw(30) << name << " | " << setw(8) << group << " | ";
	}
	~Student() {};
};


