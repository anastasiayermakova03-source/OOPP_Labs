#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Student
{
	string id;
	string name;
	int group;

public:
	Student() : id(0), name("Нет имени"), group(0) {}
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
	void setId(string id) {
		this->id = id;
	}
	void setName(string n) {
		name = n;
	}
	void setGroup(int g) {
		group = g;
	}
	void printStudent() const {
		cout << " | " << setw(6) << id << " | " << setw(30) << name << " | " << setw(8) << group << " | ";
	}
	~Student() {};
};

