#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Student.h"
#include "Test.h"
#include <ctime>
#include <sstream>

class TestResult
{
	string nameStudent;
	string nameTest;
	int maxRes;
	int curRes;
	string date;

public:
	TestResult(string st, string test, int maxRes, int curRes) {
		nameStudent = st;
		nameTest = test;
		this->curRes = curRes;
		this->maxRes = maxRes;
 		date = getCurrentTime();
	}
	TestResult(string st, string test, int maxRes, int curRes, string d) {
		nameStudent = st;
		nameTest = test;
		this->curRes = curRes;
		this->maxRes = maxRes;
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
	int getCurRes() const {
		return curRes;
	}
	int getMaxRes() const {
		return maxRes;
	}
	string getDate() const {
		return date;
	}
	void setCurRes(int res) {
		curRes = res;
	}
	void printTestResult() const {
		cout << " Студент [" << nameStudent << "]\n Тест  \"" << nameTest << "\"\n Результат [" << curRes << "] из " <<
			maxRes << "\n Время прохождения: " << date << endl;
	}
};

