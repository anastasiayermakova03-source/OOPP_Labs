#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include "TestResult.h"
using namespace std;

class FileManager
{
	public:
		static int loadStudents(vector<Student>& st) {
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
				st.push_back(temp);
			}
			file.close();
			return 0;
		}

		static int loadResults(list<TestResult>& res) {
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
				try {
					int maxRes = stoi(max);
					int curRes = stoi(r);
					TestResult temp(fullName, test, maxRes, curRes, date);
					res.push_back(temp);
				}
				catch (const exception& e) {
					cout << "Предупреждение: пропущена некорректная строка в файле results.txt -> " << line << endl;
					continue;
				}
			}
			file.close();
			return 0; 
		}
		static int loadTests(vector<Test>& tests) {
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
					for (int i = 0; i < tests.size(); ++i) {
						if (tests[i].getIdTest() == id) {
							testFound = true;
							vector<shared_ptr<TestQuestion>> qVec;
							qVec.push_back(question);
							ThematicBlock newBlock(blockName, qVec);
							tests[i].addThematicBlock(newBlock);
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
						tests.push_back(newTest);
					}
				}
				catch (const exception& e) {
					cout << "Предупреждение: пропущена некорректная строка в tests.txt -> " << line << endl;
					continue;
				}
			}
			file.close();
			return 0;
		}

		static int loadData(vector<Student>& st, vector<Test>& t, list<TestResult>& res)  {
			if (loadStudents(st) == -1 || loadTests(t) == -1 || loadResults(res) == -1)
				return -1;
		}

		static void  saveTests(vector<Test>& t) {
			if (t.empty()) {
				cout << "Ошибка записи. Передан пустой вектор тестов" << endl;
				return;
			}

			ofstream file("tests.txt");
			if (!file.is_open()) {
				cout << "Ошибка записи в файл \"tests.txt\"" << endl;
				return;
			}

			for (int i = 0; i < t.size(); i++) {
				string id = t[i].getIdTest();
				string name = t[i].getNameTest();
				vector<ThematicBlock> blocks = t[i].getBlocks(); 

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
			cout << "Все тесты успешно сохранены в файл \"tests.txt\"" << endl;
		}

		static void  saveStudents(vector<Student>& st) {
			if (st.empty()) {
				cout << "Ошибка записи. Передан пустой вектор тестов" << endl;
				return;
			}

			ofstream file("students.txt");
			if (!file.is_open()) {
				cout << "Ошибка записи в файл \"students.txt\"" << endl;
				return;
			}

			for (int k = 0; k < st.size(); ++k) {
				file << st[k].getId() << ";" << st[k].getName() << ";" << st[k].getGroup() << endl;
				}
			file.close();
			cout << "Все тесты успешно сохранены в файл \"students.txt\"" << endl;
		}

		static void  saveTestResults(list<TestResult>& res) {
			if (res.empty()) {
				cout << "Ошибка записи. Передан пустой вектор результатов тестов" << endl;
				return;
			}

			ofstream file("results.txt");
			if (!file.is_open()) {
				cout << "Ошибка записи в файл \"results.txt\"" << endl;
				return;
			}

			list<TestResult>::iterator it = res.begin();
			for (it; it != res.end(); ++it) {
				file << it->getStudent() << ";" << it->getTest() << ";" << it->getMaxRes() << ";" << it->getCurRes() << ";" << it->getDate() << endl;
			}
			file.close();
			cout << "Все тесты успешно сохранены в файл \"results.txt\"" << endl;
		}

		static void saveData(vector<Student>& st, vector<Test>& t, list<TestResult>& res) {
			saveStudents(st);
			saveTests(t);
			saveTestResults(res);
		}
};

