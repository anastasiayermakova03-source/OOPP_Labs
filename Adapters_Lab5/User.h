#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
using namespace std;

class Teacher
{
public:
	static void showMenuTeacher() {
		cout << "\n --- ћ≈Ќё ---\n";
		cout << "1) управление студентами (добавление, удаление, просмотр);\n" <<
			"2) управление тестами (добавление, удаление, просмотр);\n" <<
			"3) проверка пройденных тестов (занесение в журнал)\n" <<
			"4) результаты всех тестирований;\n" <<
			"5) поиск студентов, которые прошли конкрентый тест;\n" <<
			"6) просмотр студентов, которые должны пересдать тесты;\n" <<
			"0) выход в начальное меню;\n" ;
	}
};

class Student
{
	string id;
	string name;
	int group;
	set<string> doneTests;

public:
	Student() : id("0"), name("Ќет имени"), group(0) {}
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
	bool operator<(const Student& st) const {
		return this->getCountDoneTests() < st.getCountDoneTests();
	}
	void printStudent() const {
		cout << " | " << setw(6) << id << " | " << setw(30) << name << " | " << setw(8) << group << " | ";
	}
	static void showMenuStudent() {
		cout << "\n --- ћ≈Ќё ---\n";
		cout << "1) просмотр тестов;\n" <<
			"2) пройти тестирование (выбрать студента -> выбрать тест -> пройти);\n" <<
			"3) результаты тестирований конкретного студента;\n" <<
			"0) выход;\n";
	}
	~Student() {};
};


