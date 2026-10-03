#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "Test.h"
using namespace std;

class TestCollection
{
	string nameTestCollection;
	int count;
	Test** tests;
public:
	TestCollection() {
		count = 0;
		tests = nullptr;
	}
	TestCollection(string n, int c, Test** t) {
		if (c < 1 || t == nullptr) {
			cout << "Ошибка создания [TestCollection]" << endl;
			count = 0;
			tests = nullptr;
			return;
		}
		nameTestCollection = n;
		count = c;
		tests = new Test * [count];
		for (int i = 0; i < count; i++)
			tests[i] = t[i];
	}

	void setNameTestCollection(string name) { nameTestCollection = name; }
	void setCount(int c) { count = c; }
	int getCount() const { return count; }
	string getNameTestCollection() const { return nameTestCollection; }
	Test* getTestItem(int i) {
		if (i >= count || i < 0) {
			cout << "Ошибка. Такого теста нет" << endl;
			return nullptr;
	    }
		return tests[i];
    }

	void addTest(Test* t) {
		if (t == nullptr) {
			cout << "Ошибка. Вы пытались добавить в набор тестов пустой указатель" << endl;
			return;
		}
		Test** newTest = new Test * [count + 1];
		for (int i = 0; i < count; i++) 
			newTest[i] = tests[i];
		newTest[count++] = t;
		delete[] tests;
		tests = newTest;
	}

	void deleteTest(Test* t) {
		if (t == nullptr) {
			cout << "Ошибка. Вы пытались удалить пустой указатель из набора тестов [" << getNameTestCollection() << "]" << endl;
			return;
		}
		bool flag = false;
		Test** newTest = new Test * [count - 1];
		for (int i = 0, j = 0; i < count; i++)
			if (tests[i] == t)
				flag = true;
			else
				if (j < count-1)
				newTest[j++] = tests[i];
		if (flag) {
			count--;
			delete[] tests;
            tests = newTest;
		}
		else {
			cout << "[" << t->getFullName() << "] нет среди тестов данного набора" << endl;
			delete[] newTest;
		}
	}

	void print() const {
		if (count == 0) {
			cout << "Этот набор тестов пуст." << endl;
			return;
		}
		cout << " <<< " << nameTestCollection << " >>> " << endl;
		for (int i = 0; i < count; i++)
			tests[i]->print();
	}

	~TestCollection() {
		if (tests != nullptr) {
			for (int i = 0; i < count; i++)
				delete tests[i];
			delete[] tests;
		}
	}
};


class TestRunner {
	TestCollection* myTest;
	double success;
public:
	TestRunner() {
		myTest = nullptr;
		success = 0;
	}
	TestRunner(TestCollection* t) {
		myTest = t;
		success = 0;
	}

	TestCollection* getTestCollection() const { return myTest; }
	string getSuccess() const { return to_string(success) + "%"; }

	void calcSuccess() {
		int count_pass = 0;
		for (int i = 0; i < myTest->getCount(); i++) {
			Test* t = myTest->getTestItem(i);
			if (t->getStatus() == pass)
				count_pass++;
		}
		success = count_pass / myTest->getCount() * 100.0;
	}

	void run() {
		int count_pass = 0;
		for (int i = 0; i < myTest->getCount(); i++) {
			Test* t = myTest->getTestItem(i);
			if (t != nullptr)
			  t->execute();
		}
		cout << "Выполнение набора тестов [" << myTest->getNameTestCollection() << "] закончено." << endl;
		cout << "Результат выполнения = " << getSuccess() << endl;
	}
	~TestRunner() {};
};


class FileManager {
public:
	int loadTest(Test**& test) {
		ifstream file("AllTests.txt");
		if (!file.is_open()) {
			cout << "Ошибка чтения из файла \"AllTests.txt\"" << endl;
			return 0;
		}

		int count_test = 0;
		string line;
		while (getline(file, line))
			if (!line.empty()) count_test++;
		file.clear();
		file.seekg(0);

		test = new Test* [count_test];
		int i = 0;

		while (getline(file, line) && i<count_test) {
				if (line.empty()) continue;

				stringstream ss(line);
				string type, id, name, descr, statusStr;
				getline(ss, type, ';');
				getline(ss, id, ';');
				getline(ss, name, ';');
				getline(ss, descr, ';');
				getline(ss, statusStr, ';');

				Status status;
				if (statusStr == "1") status = failure;
				else 
				if (statusStr == "0") status = pass;
				else {
					for (int k = 0; k < i; k++) delete test[k];
					delete[] test;
					test = nullptr;
					file.close();
					return -1;
				}

				if (type == "F") {
					string excRes, curRes;
					getline(ss, excRes, ';');
					getline(ss, curRes, ';');
					test[i] = new FunctionalTest(id, name, descr, status, excRes, curRes);
				}
				if (type == "P") {
					string maxTime, curTime;
					getline(ss, maxTime, ';');
					getline(ss, curTime, ';');
					test[i] = new PerformanceTest(id, name, descr, status, stoi(maxTime), stoi(curTime));
				}
				i++;
		}
		file.close();
		cout << "Все тесты успешно считаны из файла \"AllTests.txt\"" << endl;
		return count_test;
	}


	void  saveTest(Test** test, int count_test) {
		if (test == nullptr) {
			cout << "Ошибка записи. Передан пустой массив тестов" << endl;
			return;
		}

		ofstream file("AllTests.txt");
		if (!file.is_open()) {
			cout << "Ошибка записи в файл \"AllTests.txt\"" << endl;
			return;
		}
	
		for (int i = 0; i < count_test; i++) {
			if (test[i] != nullptr)
			file << test[i]->saveTest() << endl;
		}
		file.close();
		cout << "Все тесты успешно сохранены в файл \"AllTests.txt\"" << endl;
	}


	int loadTestCollection(TestCollection**& myTestCol, Test** test, int count_test) {
		ifstream file("AllCollections.txt");
		if (!file.is_open()) {
			cout << "Ошибка чтения из файла \"AllCollections.txt\"" << endl;
			return 0;
		}

		string line;
		getline(file, line);
		int count_coll = stoi(line);
		myTestCol = new TestCollection * [count_coll];

		for (int j =0; j< count_coll; j++) {
		
		myTestCol[j] = new TestCollection();

		  if (getline(file, line) && !line.empty())
			 myTestCol[j]->setNameTestCollection(line);
		  int n = 0;
		  if (getline(file, line))
			  n = stoi(line);

		  for (int i=0; i<n; i++) {
		    getline(file, line);
			if (line.empty()) continue;
			stringstream ss(line);
			string id;
			getline(ss, id, ';');

			int index = -1;  //поиска теста в хранилище по id
			for (int x = 0; x < count_test; x++) {
				if (test[x] == nullptr) continue;
				if (id == test[x]->getId()) {
					index = x;
					break;
				}
			}
			if (index == -1) {
				cout << "Ошибка чтения файла \"AllCollections.txt\"" << endl;
				for (int k = 0; k <= j; k++) 
					delete myTestCol[k];
				delete[] myTestCol;
				myTestCol = nullptr;
				file.close();
				return -1;
			}

			if (test[index]->getType() == "F") {
				string excRes, curRes;
				getline(ss, excRes, ';');
				getline(ss, curRes, ';');
				myTestCol[j]->addTest(new FunctionalTest(id, test[index]->getName(), test[index]->getDescr(), test[index]->getStatus(), excRes, curRes));
			}
			if (test[index]->getType() == "P") {
				string maxTime, curTime;
				getline(ss, maxTime, ';');
				getline(ss, curTime, ';');
				myTestCol[j]->addTest(new PerformanceTest(id, test[index]->getName(), test[index]->getDescr(), test[index]->getStatus(), stoi(maxTime), stoi(curTime)));
			}
		  }
		}
		file.close();
		cout << "Все наборы тестов успешно считаны из файла \"AllCollections.txt\"" << endl;
		return count_coll;  //возвращаем количество считанных наборов тестов
	}


	void  saveTestCollection(TestCollection** myTestCol, int count_coll) { // count_coll - коллекции
		if (myTestCol == nullptr || count_coll <= 0) {
			cout << "Ошибка записи. Передан пустой массив c наборами тестов" << endl;
			return;
		}
		ofstream file("AllCollections.txt");
		if (!file.is_open()) {
			cout << "Ошибка записи в файл \"AllCollections.txt\"" << endl;
			return;
		}

		file << count_coll << endl;
		for (int i = 0; i < count_coll; i++) {
			if (myTestCol[i] == nullptr) continue;
			file << myTestCol[i]->getNameTestCollection() << "\n" << myTestCol[i]->getCount() << endl; // запись названия набора и кол-во его элементов

			for (int j = 0; j < myTestCol[i]->getCount(); j++) {
				file << myTestCol[i]->getTestItem(j)->getId() << ";";
				file << myTestCol[i]->getTestItem(j)->saveTestCollection() << endl;
			}
		}
		file.close();
		cout << "Все наборы тестов успешно сохранены в файл \"AllCollections.txt\"" << endl;
	}


	void  saveDeleteColl(TestCollection** myTestCol, int count_coll, int num) { // num - номер, который надо удалить
		if (myTestCol == nullptr || count_coll <= 0) {
			cout << "Ошибка записи. Передан пустой массив c наборами тестов" << endl;
			return;
		}
		ofstream file("AllCollections.txt");
		if (!file.is_open()) {
			cout << "Ошибка записи в файл \"AllCollections.txt\"" << endl;
			return;
		}

		file << count_coll-1 << endl;
		for (int i = 0; i < count_coll; i++) {
			if (num == i) continue;
			if (myTestCol[i] == nullptr) continue;
			
			file << myTestCol[i]->getNameTestCollection() << "\n" << myTestCol[i]->getCount() << endl; // запись названия набора и кол-во его элементов
			for (int j = 0; j < myTestCol[i]->getCount(); j++) {
				file << myTestCol[i]->getTestItem(j)->getId() << ";";
				file << myTestCol[i]->getTestItem(j)->saveTestCollection() << endl;
			}
		}
		file.close();
		cout << "Все наборы тестов успешно сохранены в файл \"AllCollections.txt\" после удаления теста [" << num+1<< "]\n";
	}
};

