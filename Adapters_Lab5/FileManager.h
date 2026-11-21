#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include "Test.h"
#include "User.h"
#include "TestResult.h"
using namespace std;

class FileManager
{
public:
	static int loadStudents(unordered_map<string, Student>& st) {
		ifstream file("students.txt");
		if (!file.is_open()) {
			cout << "Ошибка чтения из файла \"students.txt\"" << endl;
			return -1;
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;

			stringstream ss(line);
			string  id, fullName, gr;
			getline(ss, id, ';');
			getline(ss, fullName, ';');
			getline(ss, gr);
			Student temp(id, fullName, stoi(gr));
			st.emplace(id, temp);
		}
		file.close();
		return 0;
	}

	static int loadResults(multimap<string, TestResult>& res, unordered_map<string, Student>& st, map<string, Test>& t, queue<TestResult>& que) {
		ifstream file("results.txt"); // добавить в парамерты очередь и очередь с приоритетом
		if (!file.is_open()) {
			cout << "Ошибка чтения из файла \"results.txt\"" << endl;
			return -1;
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;

			stringstream ss(line);
			string fullName, test, r, date, status;
			getline(ss, fullName, ';');
			getline(ss, test, ';');
			getline(ss, r, ';');
			getline(ss, date, ';');
			getline(ss, status);
			string idSt = "";
			try {
				int curRes = stoi(r);
				TestResult temp(fullName, test, curRes, stoi(status), date);
				for (unordered_map<string, Student>::iterator it = st.begin(); it != st.end(); ++it) {
					if (it->second.getName() == fullName) {
						idSt = it->second.getId();
						break;
					}
				}
				if (!idSt.empty()) {
					res.emplace(idSt, temp);
					if (temp.getStatus() == 0) {
						que.push(temp);
					}
				}
					
				auto result = res.find(idSt);
				auto curSt = st.find(idSt);
				if (curSt != st.end()) {
					map<string, Test>::iterator k = t.begin();
					string idTest;
					for (k; k != t.end(); ++k)
						if (k->second.getTitle() == test) {
							idTest = k->second.getIdTest();
							break;
						}
					curSt->second.addDoneTest(idTest);
				}
				else cout << "Студент '" << fullName << "' не найден в базе студентов" << endl;

			}
			catch (const exception& e) {
				cout << "Предупреждение: пропущена некорректная строка в файле results.txt -> " << line << endl;
				continue;
			}
		}
		file.close();
		return 0;
	}

	static int loadTests(map<string, Test>& tests) {
		ifstream file("tests.txt");
		if (!file.is_open()) {
			cout << "Ошибка чтения из файла \"tests.txt\"" << endl;
			return -1;
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;

			stringstream ss(line);
			string id, title, numQ;
			getline(ss, id, ';');
			getline(ss, title, ';');
			getline(ss, numQ, ';');

			vector<Question> q;
			string str, idQ, text, opt, correct;
			array<string, 4> options;

			for (int i = 0; i < stoi(numQ); ++i) {
				getline(file, str);
				if (str.empty()) { --i; continue; }
				stringstream qq(str);

				getline(qq, idQ, ';');
				getline(qq, text, ';');
				for (int t = 0; t < 4; t++) {
					getline(qq, opt, ';');
					options[t] = opt;
				}
				getline(qq, correct, ';');
				q.push_back(Question(stoi(idQ), text, options, stoi(correct)));
			}
			tests[id] = Test(id, title, q);
		}

		file.close();
		return 0;
	}

	static int loadData(unordered_map<string, Student>& st, map<string, Test>& t, multimap<string, TestResult>& res, queue<TestResult>& que) {
		if (loadStudents(st) == -1 || loadTests(t) == -1 || loadResults(res, st, t, que) == -1)
			return -1;
	}


	static void  saveTests(map<string, Test>& t) {
		if (t.empty()) {
			cout << "Ошибка записи. Передан пустой список тестов" << endl;
			return;
		}
		ofstream file("tests.txt");
		if (!file.is_open()) {
			cout << "Ошибка записи в файл \"tests.txt\"" << endl;
			return;
		}

		for (map<string, Test>::iterator it = t.begin(); it != t.end(); ++it)
			it->second.saveTest(file);	
		file.close();
	}

	static void  saveStudents(unordered_map<string, Student>& st) {
		if (st.empty()) {
			cout << "Ошибка записи. Передан пустой вектор тестов" << endl;
			return;
		}

		ofstream file("students.txt");
		if (!file.is_open()) {
			cout << "Ошибка записи в файл \"students.txt\"" << endl;
			return;
		}

		for (unordered_map<string, Student>::iterator it = st.begin(); it != st.end(); ++it) {
			file << it->first << ";" << it->second.getName() << ";" << it->second.getGroup() << endl;
		}
		file.close();
	}

	static void  saveTestResults(multimap<string, TestResult>& res) {
		if (res.empty()) {
			cout << "Ошибка записи. Передан пустой вектор результатов тестов" << endl;
			return;
		}

		ofstream file("results.txt");
		if (!file.is_open()) {
			cout << "Ошибка записи в файл \"results.txt\"" << endl;
			return;
		}

		multimap<string, TestResult>::iterator it = res.begin();
		for (it; it != res.end(); ++it) {
			file << it->second.getStudent() << ";" << it->second.getTest() << ";" << it->second.getCurRes() << ";" << it->second.getDate() << ";" << it->second.getStatus() << endl;
		}
		file.close();
	}

	static void saveData(unordered_map<string, Student>& st, map<string, Test>& t, multimap<string, TestResult>& res) {
		saveStudents(st);
		saveTests(t);
		saveTestResults(res);
	}
};



