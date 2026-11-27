#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Student.h"
#include "Test.h"
#include <ctime>
#include <random>
#include <sstream>
#include <iostream>
#include <stdexcept>

class TestResult
{
	int id;
	string nameStudent;
	string nameTest;
	int curRes;
	string date;

	TestResult(string st, string test, int curRes) { // для корректного создания новвых результатов через create() 
		this->id = generateResId();
		nameStudent = st;
		nameTest = test;
		this->curRes = curRes;
		date = getCurrentTime();
	}

public:
	TestResult(int id, string st, string test, int curRes, string d) {
		this->id = id;
		nameStudent = st;
		nameTest = test;
		this->curRes = curRes;
		date = d;
	}

	static TestResult create(const string& st, const string& test, int curRes) { // фабричный конструктор для проверки валидности данных
		if (st.empty() || test.empty())
			throw invalid_argument("Не указан студент или тест");
		if (curRes < 0 || curRes > 10)
			throw invalid_argument("Результат должен быть в диапазоне 0..10");
		return TestResult(st, test, curRes);
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
	int getResId() const {
		return id;
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
	string getDate() const {
		return date;
	}
	void setCurRes(int res) {
		curRes = res;
	}
	int generateResId() {
		static mt19937 gen(random_device{}());
		return static_cast<int>(gen() >> 1);
	}
	void printTestResult() const {
		cout << " Студент [" << nameStudent << "]\n Тест  \"" << nameTest << "\"\n Результат [" << curRes << "] из 10.\n Время прохождения: " << date << endl;
	}
};


