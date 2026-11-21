#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "User.h"
#include "Test.h"
#include <ctime>
#include <sstream>

enum Status {Unchecked, Checked};

class TestResult
{
	string nameStudent;
	string nameTest;
	double curRes;
	string date;
	Status status;

public:
	TestResult(string st, string test, double curRes) {
		nameStudent = st;
		nameTest = test;
		this->curRes = curRes;
		status = Status::Unchecked;
		date = getCurrentTime();
	}
	TestResult(string st, string test, double curRes, int s, string d) {
		nameStudent = st;
		nameTest = test;
		this->curRes = curRes;
		if (s) status = Status::Checked;
		else status = Status::Unchecked;
		date = d;
	}
	string getCurrentTime() {
		// 1. Получаем текущее системное время в виде числа секунд
		time_t now = std::time(nullptr);
		// 2. Превращаем это число в структуру с разбивкой на часы, дни, годы
		tm ltm;
		localtime_s(&ltm, &now);
		// 3. Записываем в строку по нашему формату
		stringstream ss;
		ss << put_time(&ltm, "%d.%m.%Y %H:%M");
		return ss.str();
	}
	string getStudent() const {
		return nameStudent;
	}
	string getTest() const {
		return nameTest;
	}
	double getCurRes() const {
		return curRes;
	}
	int getStatus() const {
		return status;
	}
	string getDate() const {
		return date;
	}
	void setCurRes(double res) {
		curRes = res;
	}
	void setStatus(int st) {
		status = Status::Checked;
	}
	void printTestResult() const {
		cout << " | Студент [" << nameStudent << "]\n | Тест  \"" << nameTest << "\"\n | Результат [" << setprecision(1) << curRes << "] из 10" <<
		"\n | Время прохождения: " << date << endl;
	}

};



