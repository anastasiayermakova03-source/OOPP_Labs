#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <stdexcept>
#include <algorithm>
#include "TestResult.h"
#include "FileManager.h"
using namespace std;

class TestingManager
{
	unordered_map<string,Student> students;
	map<string,Test> tests;
	multimap<string,TestResult> results;

public:
	TestingManager() {
		int loading = FileManager::loadData(students, tests, results);
		if (loading == -1) {
			cout << "Дальнейшая работа невозможна, устраните ошибки с файлами\n";
			throw runtime_error("Ошибка работы с файлом");
		}
		else
			cout << "Данные с текстовых файлов успешно загружены\n";
	}
	~TestingManager() {
		FileManager::saveData(students, tests, results);
		cout << "Данные успешно сохранены в текстовые файлы\n";
	}

	int insideMenu() {
		int ch;
		cout << "1) добавление;\n" <<
			"2) удаление;\n" <<
			"3) просмотр;\n" <<
			" Выбор: ";
		cin >> ch;
		if (!cin.good()) {
			cout << "Возникла проблема" << endl;
			cin.clear();
			cin.ignore(100, '\n');
			return -1;
		}
		if (ch > 3 || ch < 1) {
			cout << "Некорректный ввод числового значения (1-3)" << endl;
			return -1;
		}
		return ch;
	}

	void addStudent() {
		string fullName, id;
		int group;
		cin.ignore();
		cout << string(15, '-') << endl;
		cout << " Введите ID студента: " << endl;
		getline(cin, id);
		cout << " ФИО студента: " << endl;
		getline(cin, fullName);
		cout << " Группа (5 цифр): " << endl;
		cin >> group;
		cin.ignore();
		Student st(id, fullName, group);
		students.emplace(id, st);
		cout << " Новый студент ->\n";
		st.printStudent();
	}

	void deleteStudent() {
		cout << " --- СТУДЕНТЫ --- " << endl;
		cout << string(56, '-') << endl;
		cout << "№ | " << setw(6) << "Номер" << " | " << setw(30) << "ФИО" << " | " << setw(8) << "Группа" << " | " << endl;
		int ind = 1;
		for (unordered_map<string, Student>::iterator it = students.begin(); it != students.end(); ++it) {
			cout << ind++;
			it->second.printStudent();
			cout << endl;
		}
		cout << string(56, '-') << endl;
		int ch;
		cin.ignore();
		cout << "\n Введите № студента из таблицы: " << endl;
		cin >> ch;

		if (ch > students.size() || ch < 1) {
			cout << "Некорректный ввод номера студента (1-" << students.size() << ")" << endl;
			return;
		}
		unordered_map<string, Student>::iterator it = students.begin();
		unordered_map<string, Student>::iterator itName = next(it,ch-1);
		string nameSt = itName->second.getName();
		string idSt = itName->first;
		students.erase(itName);
		
		multimap<string,TestResult>::iterator r = results.begin();
		for (r; r != results.end();)
			if (idSt == r->first)
				r = results.erase(r);
			else r++;
		
		cout << " Cтудент [" << nameSt << "] успешно удален\n";
	}

	void addTest() {
		string name, id;
		string blockName;
		cout << string(15, '-') << endl;
		cin.ignore();
		cout << " Введите ID теста: " << endl;
		getline(cin, id);
		cout << " Название теста: " << endl;
		getline(cin, name);
		cout << " Название тематического блока: " << endl;
		getline(cin, blockName);
		cout << " Необходимо заполнить блок [" << blockName << "] вопросами" << endl;
		cout << string(15, '-') << endl;

		int type, max, choice;
		string text;
		vector<shared_ptr<TestQuestion>> blockQuestions;

		while (true) {
			cout << " Выберите тип вопроса (1 - несколько вариантов ответа; 2 - самостоятельный вариант ответа): ";
			cin >> type;
			cin.ignore();
			if (!cin.good()) {
				cout << "Возникла проблема с потоком ввода" << endl;
				cin.clear();
				cin.ignore(100, '\n');
				cout << "Можете продолжать работу\n";
				continue;
			}
			if (type != 2 && type != 1) {
				cout << "Некорректный ввод числового значения (1 или 2)" << endl;
				continue;
			}
			cout << " Введите текст вопроса: ";
			getline(cin, text);
			cout << " Введите максимальное кол-во баллов за правильный ответ (целое число 1-10): ";
			cin >> max;
			cin.ignore();

			if (type == 1) {
				array<string, 4> opt;
				int right;
				for (int i = 0; i < 4; ++i) {
					cout << " Вариант ответа [" << i + 1 << "]: ";
					getline(cin, opt[i]);
				}
				cout << " Номер правильного ответа (1-4): ";
				cin >> right;
				cin.ignore();
				shared_ptr< ChoiceQuestion> q = make_shared<ChoiceQuestion>(text, max, opt, right);
				blockQuestions.push_back(q);
				cout << " Вопрос добавлен в блок [" << blockName << "]" << endl;
			}
			else if (type == 2) {
				string opt;
				cout << " Правильный вариант ответа: ";
				getline(cin, opt);
				shared_ptr<DefQuestion> q = make_shared<DefQuestion>(text, max, opt);
				blockQuestions.push_back(q);
				cout << " Вопрос добавлен в блок [" << blockName << "]" << endl;
			}

			cout << " Продолжить добавлять вопросы? (1 - да, 2 - нет): ";
			cin >> choice;
			cin.ignore();

			if (!cin.good()) {
				cout << "Возникла проблема с потоком ввода" << endl;
				cin.clear();
				cin.ignore(100, '\n');
				break;
			}
			if (choice == 2) {
				break;
			}
		}

		if (!blockQuestions.empty()) {
			ThematicBlock block(blockName, blockQuestions);
			vector<ThematicBlock> blocks;
			blocks.push_back(block);
			Test newTest(id, name, blocks);
			tests.emplace(id,newTest);
			cout << "\n Тест [" << name << "] успешно создан!" << endl;
		}
	}

	void deleteTest() {
		int numTest;
		cout << "\n ------- ТЕСТЫ В СИСТЕМЕ ------- " << endl;
		cout << string(45, '-') << endl;
		int ind = 1;
		map<string, Test>::iterator t = tests.begin();
		for (t; t != tests.end(); ++t) {
			cout << " [" << ind++ << "]";
			t->second.printTest();
			cout << endl;
		}
		cout << string(45, '-') << endl << " Выберите номер теста, который нужно удалить: ";
		cin >> numTest;
		if (numTest > tests.size() || numTest < 1) {
			cout << "Некорректный ввод номера теста (1-" << tests.size() << ")" << endl;
			return;
		}

		t = tests.begin();
		map<string, Test>::iterator delT = next(t, numTest - 1);
		string delName = delT->second.getNameTest();
		string delId = delT->first;
		tests.erase(delT);

		multimap<string, TestResult>::iterator r = results.begin();
		for (r; r != results.end(); )
			if (r->second.getTest() == delName)
				r = results.erase(r);
			else r++;

		unordered_map<string, Student>::iterator s = students.begin();
		for (s; s != students.end(); ++s) {
			set<string> doneTests = s->second.getDoneTests();
			if (doneTests.erase(delId)>0) {
				s->second.addFullDoneTest(doneTests);
			}
		}
		cout << "Тест [" << delName << "] успешно удален из системы" << endl;
	}


	void run() {
		cout << "\n === Учет проведения тестирования по программированию среди студентов ===\n";
		int choice;
		bool flag = 1;
		while (flag) {
			cout << "\n --- МЕНЮ ---\n";
			cout << "1) управление студентами (добавление, удаление, просмотр);\n" <<
				"2) управление тестами (добавление, удаление, просмотр);\n" <<
				"3) пройти тестирование (выбрать студента -> выбрать тест -> пройти);\n" <<
				"4) результаты тестирований;\n" <<
				"5) поиск студентов, которые прошли конкрентый тест;\n" <<
				"6) сортировка студентов по количеству пройденных тестов;\n" <<
				"7) выход;\n" <<
				" Выбор: ";
			cin >> choice;
			if (!cin.good()) {
				cout << "Возникла проблема с потоком ввода" << endl;
				cin.clear();
				cin.ignore(100, '\n');
				cout << "Можете продолжать работу\n";
				continue;
			}
			if (choice > 7 || choice < 1) {
				cout << "Некорректный ввод числового значения (1-5)" << endl;
				continue;
			}

			switch (choice) {
			case 1: {
				cout << "\n == УПРАВЛЕНИЕ СТУДЕНТАМИ ==\n";
				int res = insideMenu();
				if (res == -1) break;
				if (res == 1)  addStudent(); 
				if (res == 2) deleteStudent(); // удаление студента
				if (res == 3) {
					cout << " --- СТУДЕНТЫ --- " << endl;
					cout << string(56, '-') << endl;
					cout << "№ | " << setw(6) << "Номер" << " | " << setw(30) << "ФИО" << " | " << setw(8) << "Группа" << " | " << endl;
					int ind = 1;
					for (unordered_map<string, Student>::iterator it = students.begin(); it != students.end(); ++it) {
						cout << ind++;
						it->second.printStudent();
						cout << endl;
					}
					cout << string(56, '-') << endl;
				}
				break;
			}
			case 2: {
				cout << "\n == УПРАВЛЕНИЕ ТЕСТАМИ ==\n";
				int res = insideMenu();
				if (res == -1) break;
				if (res == 1)  addTest();
			    if (res == 2) deleteTest(); // удаление теста
				if (res == 3) {
					cout << " ------- ТЕСТЫ В СИСТЕМЕ ------- " << endl;
					cout << string(45, '-') << endl;
					int ind = 1;
					for (map<string, Test>::iterator t = tests.begin(); t != tests.end(); ++t) {
						cout << " [" << ind++ << "]";
						t->second.printTest();
						cout << endl;
					}
					cout << string(45, '-') << endl;
				}
				break;
			}
			case 3: {
				cout << "\n == ПРОХОЖДЕНИЕ ТЕСТИРОВАНИЯ ==\n";
				int numSt, numTest, res;

				cout << " --- СТУДЕНТЫ --- " << endl;
				cout << string(56, '-') << endl;
				cout << "№ | " << setw(6) << "Номер" << " | " << setw(30) << "ФИО" << " | " << setw(8) << "Группа" << " | " << endl;
				int ind = 1;
				unordered_map<string, Student>::iterator it = students.begin();
				for (it; it != students.end(); ++it) {
					cout << ind++;
					it->second.printStudent();
					cout << endl;
				}
				cout << string(56, '-') << endl << " Выберите номер студента: ";
				cin >> numSt;
				if (numSt > students.size() || numSt < 1) {
					cout << "Ошибка: Некорректный ввод номера студента (1-" << students.size() << ")" << endl;
					break;
				}

				it = students.begin();
				unordered_map<string,Student>::iterator newSt = next(it, numSt-1);
				
				cout << "\n ------- ТЕСТЫ В СИСТЕМЕ ------- " << endl;
				cout << string(45, '-') << endl;
				ind = 1;
				for (map<string, Test>::iterator t = tests.begin(); t != tests.end(); ++t) {
					cout << " [" << ind++ << "]";
					t->second.printTest();
					cout << endl;
				}
				cout << string(45, '-') << endl << " Выберите номер теста: ";
				cin >> numTest;
				if (numTest > tests.size() || numTest < 1) {
					cout << "Некорректный ввод номера теста (1-" << tests.size() << ")" << endl;
					break;
				}

                map<string, Test>::iterator t = tests.begin();
				auto newT = next(t, numTest - 1);
				cout << " Введите результат прохождения теста (целое число в диапазоне 1 - " << newT->second.getScore() <<"): ";
				cin >> res;
				if (res > newT->second.getScore()) {
					cout << "Некорректный ввод балла (1-" << newT->second.getScore() << ")" << endl;
					break;
				}
				newSt->second.addDoneTest(newT->second.getIdTest()); //сохранение теста в set студента

				TestResult result(newSt->second.getName(), newT->second.getNameTest(), newT->second.getScore(), res);
				results.emplace(newSt->first,result);
				cout << "\n Тест [" << newT->second.getNameTest() << "] успешно пройден от лица студента[" <<
					newSt->second.getName() << "] на балл = " << res << endl;
				break;
			}
			case 4: {
				cout << "\n == РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЙ ==\n";
				cout << string(50, '-') << endl;
				multimap<string,TestResult>::iterator r = results.begin();
				int ind = 1;
				for (r; r != results.end(); ++r) {
					cout << "---> [" << ind++ << "]" << endl;
					r->second.printTestResult();
					cout << endl;
				}
				cout << string(50, '-') << endl;
				break;
			}
			case 5: {
				cout << "\n\t == ПОИСК ==\n";
				cout << "\n ------- ТЕСТЫ В СИСТЕМЕ ------- " << endl;
				cout << string(45, '-') << endl;
				int ind = 1;
				int numTest;
				map<string, Test>::iterator t = tests.begin();
				for (t; t != tests.end(); ++t) {
					cout << " [" << ind++ << "]";
					t->second.printTest();
					cout << endl;
				}
				cout << string(45, '-') << endl << " Выберите номер теста: ";
				cin >> numTest;
				if (numTest > tests.size() || numTest < 1) {
					cout << "Некорректный ввод номера теста (1-" << tests.size() << ")" << endl;
					break;
				}

				t = tests.begin();
				auto curTest = next(t, numTest - 1);
				string id = curTest->first;
				bool flag = 0;
				cout << "\n Студенты, которые прошли Тест [" << curTest->second.getNameTest() << "] :" << endl;
				unordered_map<string, Student>::iterator curSt = students.begin();
				for (curSt; curSt != students.end(); ++curSt) {
					if (curSt->second.getDoneTests().count(id) > 0) {
						cout << " - " << curSt->second.getName() << endl;
						flag = true;
					}
				}
				if (!flag) cout << "Все студенты НЕ прошли этот Тест" << endl;
				break;
			}
			case 6: {
				cout << "\n == СОРТИРОВКА СТУДЕНТОВ ==\n";
				cout << " по количеству пройденных тестов (по убыванию) \n";
				cout << string(50, '-') << endl;
				
				vector<pair<string,Student>> sortedSt(students.begin(),students.end()); //создаем вектор из пар
				sort(sortedSt.begin(), sortedSt.end(), [](const pair<string, Student>& a, const pair<string, Student>& b) {
					return a.second.getDoneTests().size() > b.second.getDoneTests().size();
					});

				for (int i = 0; i < sortedSt.size(); ++i) {
					cout << "'" << sortedSt[i].second.getName() << "' - " << sortedSt[i].second.getCountDoneTests() << " тест(-ов)" << endl;
				}
				break;
			}
			case 7:
				cout << "\n ВЫХОД ИЗ ПРОГРАММЫ...\n";
				flag = 0;
				break;
			}
		}
	}
};


