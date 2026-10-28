#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>
#include <set>
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

	static int loadResults(multimap<string, TestResult>& res,unordered_map<string, Student>& st, map<string, Test>& t) {
		ifstream file("results.txt");
		if (!file.is_open()) {
			cout << "Ошибка чтения из файла \"results.txt\"" << endl;
			return -1;
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;

			stringstream ss(line);
			string fullName, test, max, r, date;
			getline(ss, fullName, ';');
			getline(ss, test, ';');
			getline(ss, max, ';');
			getline(ss, r, ';');
			getline(ss, date);
			string idSt = "";
			try {
				int maxRes = stoi(max);
				int curRes = stoi(r);
				TestResult temp(fullName, test, maxRes, curRes, date);
				for (unordered_map<string, Student>::iterator it = st.begin(); it != st.end(); ++it) {
					if (it->second.getName() == fullName) {
						idSt = it->second.getId();
						break;
					}
				}
				if (!idSt.empty())
				res.emplace(idSt,temp);
				auto curSt = st.find(idSt);
				if (curSt != st.end()) {
					map<string, Test>::iterator k = t.begin();
					string idTest;
					for (k; k != t.end(); ++k)
						if (k->second.getNameTest() == test) {
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

	static int loadTests(map<string,Test>& tests) {
		ifstream file("tests.txt");
		if (!file.is_open()) {
			cout << "Ошибка чтения из файла \"tests.txt\"" << endl;
			return -1;
		}

		string line;
		while (getline(file, line)) {
			if (line.empty()) continue;

			try {
				stringstream ss(line);
				string id, name, blockName, qTypeStr, qText, qMaxScore, rightAns;

				getline(ss, id, ';');
				getline(ss, name, ';');
				getline(ss, blockName, ';');
				getline(ss, qTypeStr, ';');
				getline(ss, qText, ';');
				getline(ss, qMaxScore, ';');

				int type = stoi(qTypeStr);
				int maxScore = stoi(qMaxScore);

				shared_ptr<TestQuestion> question = nullptr;

				if (type == 1) {
					array<string, 4> opts;
					for (int i = 0; i < 4; ++i) {
						getline(ss, opts[i], ';');
					}
					getline(ss, rightAns, ';');
					int right = stoi(rightAns);
					question = make_shared<ChoiceQuestion>(qText, maxScore, opts, right);
				}
				else if (type == 2) {
					getline(ss, rightAns, ';');
					question = make_shared<DefQuestion>(qText, maxScore, rightAns);
				}

				bool testFound = false;
				for (map<string, Test>::iterator it = tests.begin(); it != tests.end(); ++it) {
					if (it->second.getIdTest() == id) {
						testFound = true;
						vector<ThematicBlock>& bl = it->second.getBlocks();
						bool blockFound = false;
						for (int i = 0; i < bl.size(); ++i) {
							if (bl[i].getTopic() == blockName) {
								blockFound = true;
								bl[i].addQuestions(question);
								break;
							}
						}
						if (!blockFound) {
							vector<shared_ptr<TestQuestion>> qVec = { question };
							ThematicBlock newBlock(blockName, qVec);
							bl.push_back(newBlock);
						}
						break;
					}
				}

				if (!testFound) {
					vector<shared_ptr<TestQuestion>> qVec;
					qVec.push_back(question);
					ThematicBlock block(blockName, qVec);
					vector<ThematicBlock> blocks;
					blocks.push_back(block);
					Test newTest(id, name, blocks);
					tests.emplace(id, newTest);
				}
			}
			catch (const exception& e) {
				cout << "Предупреждение: пропущена некорректная строка в tests.txt -> " << line << endl;
				continue;
			}
		}
		for (map<string, Test>::iterator it = tests.begin(); it != tests.end(); ++it)
			it->second.setScore(it->second.calculateScore());
		file.close();
		return 0;
	}

	static int loadData(unordered_map<string,Student>& st, map<string,Test>& t, multimap<string, TestResult>& res) {
		if (loadStudents(st) == -1 || loadTests(t) == -1 || loadResults(res, st, t) == -1)
			return -1;
	}

	static void  saveTests(map<string, Test>& t) {
		if (t.empty()) {
			cout << "Ошибка записи. Передан пустой вектор тестов" << endl;
			return;
		}

		ofstream file("tests.txt");
		if (!file.is_open()) {
			cout << "Ошибка записи в файл \"tests.txt\"" << endl;
			return;
		}

		for (map<string, Test>::iterator it = t.begin(); it != t.end(); ++it) {
			string id = it->first;
			string name = it->second.getNameTest();
			vector<ThematicBlock> blocks = it->second.getBlocks();

			for (int j = 0; j < blocks.size(); ++j) {
				string blockName = blocks[j].getTopic();
				vector<shared_ptr<TestQuestion>> questions = blocks[j].getQuestions();

				for (int k = 0; k < questions.size(); ++k) {
					file << id << ";" << name << ";" << blockName << ";";
					questions[k]->saveToFile(file);
				}
			}
		}
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

	static void  saveTestResults(multimap<string,TestResult>& res) {
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
			file << it->second.getStudent() << ";" << it->second.getTest() << ";" << it->second.getMaxRes() << ";" << it->second.getCurRes() << ";" << it->second.getDate() << endl;
		}
		file.close();
	}

	static void saveData(unordered_map<string, Student>& st, map<string, Test>& t, multimap<string, TestResult>& res) {
		saveStudents(st);
		saveTests(t);
		saveTestResults(res);
	}
};


