#pragma once
#include <memory>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <set>
#include "Test.h"
#include "Student.h"
#include "TestResult.h"
using namespace std;

// Фабричный метод с параметрами - тип вопроса определяется параметром type - для считывания из файлов

class QuestionFactory
{
public:
	static unique_ptr<TestQuestion> create(const string& type, const string& text, const string& rightAns, const array<string, 4>& options = array<string, 4>{}) {
		if (type == "CHOICE") {
			int right = stoi(rightAns);
			if (right < 1 || right > 4)
				throw runtime_error ("Номер правильного варианта должен быть 1-4");
			return make_unique<ChoiceQuestion>(text, options, right);
		}
		if (type == "DEF")
			return make_unique<DefQuestion>(text, rightAns);
		return nullptr;
	}
};


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
			string  id, fullName, gr, res;
			getline(ss, id, ';');
			getline(ss, fullName, ';');
			getline(ss, gr, ';');
			getline(ss, res);
			try {
				set<string> stRes;
				if (!res.empty()) {
					string num;
					stringstream r(res);
					while (getline(r, num, '|'))
						stRes.emplace(num);
				}
				st.emplace(id, Student(id, fullName, stoi(gr), stRes));
			}
			catch (const exception&) {
				cout << "Предупреждение: пропущена некорректная строка в файле students.txt -> " << line << endl;
			}
		}
		file.close();
		return 0;
	}

	static int loadResults(multimap<string, TestResult>& res, unordered_map<string, Student>& st, map<string, Test>& t) {
		ifstream file("results.txt");
		if (!file.is_open()) {
			cout << "Ошибка чтения из файла \"results.txt\"" << endl;
			return -1;
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;

			stringstream ss(line);
			string id, fullName, test, r, date;
			getline(ss, id, ';');
			getline(ss, fullName, ';');
			getline(ss, test, ';');
			getline(ss, r, ';');
			getline(ss, date, ';');
			try {
				TestResult temp(stoi(id), fullName, test, stoi(r), date);

				string idSt;
				for (unordered_map<string, Student>::iterator it = st.begin(); it != st.end(); ++it) {
					if (it->second.getName() == fullName) {
						idSt = it->second.getId();
						break;
					}
				}
				if (idSt.empty()) {
					cout << "Студент '" << fullName << "' не найден в базе студентов" << endl;
					continue;
				}
				res.emplace(idSt, temp);

				string idTest;
				for (map<string, Test>::iterator k = t.begin(); k != t.end(); ++k) {
					if (k->second.getTitle() == test) {
						idTest = k->second.getIdTest();
						break;
					}
				}
				if (!idTest.empty())
					st.find(idSt)->second.addMyResult(idTest);
			}
			catch (const exception&) {
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

			int count = 0;
			count = stoi(numQ);
			
			Test test(id, title);
			for (int i = 0; i < count; ++i) {
				string question;
				if (!getline(file, question)) break;
				if (question.empty()) { --i; continue; }

				stringstream qs(question);
				string qTypeStr, qText, rightAns;
				getline(qs, qTypeStr, ';');
				getline(qs, qText, ';');
				getline(qs, rightAns, ';');

				array<string, 4> opts;
				if (qTypeStr == "CHOICE")
					for (int j = 0; j < 4; ++j)
						getline(qs, opts[j], ';');

				try { // использование фабричного метода с параметрами
					unique_ptr<TestQuestion> q = QuestionFactory::create(qTypeStr, qText, rightAns, opts);
					if (q) test.addTestQuestion(*q);
					else cout << "Предупреждение: неизвестный тип вопроса -> " << question << endl;
				}
				catch (const exception&) {
					cout << "Предупреждение: некорректный вопрос -> " << question << endl;
				}
			}
			tests.erase(id);
			tests.emplace(id, test);
		}
		file.close();
		return 0;
	}

	static int loadData(unordered_map<string, Student>& st, map<string, Test>& t, multimap<string, TestResult>& res) {
		if (loadStudents(st) == -1 || loadTests(t) == -1 || loadResults(res, st, t) == -1)
			return -1;
		return 0;
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
			cout << "Ошибка записи. Передан пустой список студентов" << endl;
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
			cout << "Ошибка записи. Передан пустой список результатов тестов" << endl;
			return;
		}
		ofstream file("results.txt");
		if (!file.is_open()) {
			cout << "Ошибка записи в файл \"results.txt\"" << endl;
			return;
		}
		for (multimap<string, TestResult>::iterator it = res.begin(); it != res.end(); ++it) {
			file << it->second.getResId() << ";" << it->second.getStudent() << ";" << it->second.getTest() << ";"
				<< it->second.getCurRes() << ";" << it->second.getDate() << endl;
		}
		file.close();
	}

	static void saveData(unordered_map<string, Student>& st, map<string, Test>& t, multimap<string, TestResult>& res) {
		saveStudents(st);
		saveTests(t);
		saveTestResults(res);
	}
};
