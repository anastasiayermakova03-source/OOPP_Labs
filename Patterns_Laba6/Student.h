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
	set<string> myResults;

public:
	Student() : id(0), name("Нет имени"), group(0) {}
	Student(string id, string n, int g, const set<string>& res) {
		this->id = id;
		name = n;
		group = g;
		myResults = res;
	}
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
	set<string> getMyResults() const {
		return myResults;
	}
	void addMyResult(string idRes) {
			myResults.emplace(idRes);
	}
	void deleteMyResult(string idRes) {
		auto it = myResults.find(idRes);
		if (it != myResults.end())
			myResults.erase(idRes);
		else cout << "Результата прохождения теста с ID " << idRes << " нет среди результатов студента '" << getName() << "'\n";
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
	bool operator<(const Student& other) const {
		return this->name < other.name;
	}
	void printStudent() const {
		cout << " | " << setw(6) << id << " | " << setw(30) << name << " | " << setw(8) << group << " | ";
		bool found = false;
		for (auto res = myResults.begin(); res != myResults.end(); ++res)
			if (found) cout << "," << *res;
			else { cout << *res; found = true; }
		if (found) cout << " | ";
		else cout << left << setw(10) << "-" << " | ";
	}
	~Student() {};
};

