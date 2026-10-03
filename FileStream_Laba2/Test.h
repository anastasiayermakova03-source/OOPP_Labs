#pragma once
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

enum Status {pass, failure};

class Test
{
	string id;
	string name;
	string descr;
	Status status;
public:
	Test() {
		id = "TST-01";
		name = "NoName";
		descr = "no describe";
		status = failure;
	};
	Test(string i, string n, string d, Status st) : id(i), name(n), descr(d), status(st) {}
	void setStatus(Status st) { status = st; }
	string getFullName() const { return name + " - " + descr; }
	string getName() const { return name; }
	string getDescr() const { return descr; }
	string getId() const { return id; }
	Status getStatus() const { return status; }
	virtual string getType() = 0;
	virtual string saveTest() = 0;	
	virtual string saveTestCollection() = 0;
	virtual void execute() = 0;
	virtual void print() const {
		cout << " | " << setw(7) << id << " | " << setw(25) << name << " | " << setw(55) << descr << " | \n";
	}
	virtual ~Test() {};
};

class FunctionalTest : public Test
{
	string expectedRes;
	string currentRes;
public:
	FunctionalTest() : Test() {
		expectedRes = "empty";
		currentRes = "empty";
	}
	FunctionalTest(string i, string n, string d, Status st, string exRes, string curRes) :
		Test(i, n, d, st), expectedRes(exRes), currentRes(curRes) {};
	void setExpect(string exRes) { expectedRes = exRes; }
	void setCur(string curRes) { currentRes = curRes; }
	string getCur() const { return currentRes; }
	string getExpect() const { return expectedRes; }
	string getType() override { return "F"; }
	string saveTest() override {
		return "F;" + getId() + ";" + getName() + ";" + getDescr() + ";" +
			(getStatus() == pass ? "0" : "1") + ";" + expectedRes + ";" + currentRes;
	}
	string saveTestCollection() override {
		return  expectedRes + ";" + currentRes;
	}
	void execute() override {
		if (expectedRes == currentRes)
			setStatus(pass);
		else setStatus(failure);
	}
	void print() const override {
		Test::print();
		//cout << " | " << setw(5) << expectedRes << " | " << setw(5) << currentRes << " | " << endl;
	}
	~FunctionalTest() {}
};

class PerformanceTest : public Test
{
	int maxTime;
	int currentTime;
public:
	PerformanceTest(string i, string n, string d, Status st, int max, int cur) :
		Test(i, n, d, st), maxTime(max), currentTime(cur) {
	};
	PerformanceTest() :Test() {
		maxTime = 0;
		currentTime = 0;
	};
	void setCurrentTime(int ct) { currentTime = ct; }
	int getCurrentTime() const { return currentTime; }
	int getMaxTime() const { return maxTime; }
	string getType() override { return "P"; }
	string saveTest() override {
		return "P;" + getId() + ";" + getName() + ";" + getDescr() + ";" +
			(getStatus() == pass ? "0" : "1") + ";" + to_string(maxTime) + ";" + to_string(currentTime);
	}
	string saveTestCollection() override {
		return  to_string(maxTime) + ";" + to_string(currentTime);
	}
	void execute() override {
		if (currentTime <= maxTime)
			setStatus(pass);
		else setStatus(failure);
	}
	void print() const override {
		Test::print();
		//cout << " | " << setw(5) << maxTime << " | " << setw(5) << currentTime << " | " << endl;
	}
	~PerformanceTest() {}
};

