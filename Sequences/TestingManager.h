#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <stdexcept>
#include "TestResult.h"
#include "FileManager.h"
using namespace std;

class TestingManager
{
	vector<Student> students;
	vector<Test> tests;
	list<TestResult> results;

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
		students.push_back(st);
		cout << " Новый студент ->\n";
		st.printStudent();
	}

	void deleteStudent() {
		cout << " --- СТУДЕНТЫ --- " << endl;
		cout << string(56, '-') << endl;
		cout << "№ | " << setw(6) << "Номер" << " | " << setw(30) << "ФИО" << " | " << setw(8) << "Группа" << " | " << endl;
		for (int i = 0; i < students.size(); ++i) {
			cout << i + 1;
			students[i].printStudent();
			cout << endl;
		}
		cout << string(56, '-') << endl;
		int ch;
		cin.ignore();
		cout << "\n Введите № студента из таблицы: " << endl;
		cin >> ch;

		if (ch > students.size()) {
			cout << "Некорректный ввод номера студента (1-" << students.size() << ")" << endl;
			return;
		}
		vector<Student>::iterator it = students.begin();
		string nameSt = students[ch - 1].getName();
		students.erase(it+(ch-1));
		cout << " Cтудент [" << nameSt <<"] успешно удален\n";

		list<TestResult>::iterator r = results.begin();
		for (r; r != results.end(); ++r)
			if (nameSt == r->getStudent())
				r = results.erase(r);
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
			tests.push_back(newTest);
			cout << "\n Тест [" << name << "] успешно создан!" << endl;
		}
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
				"4) результаты тестирований ;\n" <<
				"5) выход;\n" <<
				" Выбор: ";
			cin >> choice;
			if (!cin.good()) {
				cout << "Возникла проблема с потоком ввода" << endl;
				cin.clear();
				cin.ignore(100, '\n');
				cout << "Можете продолжать работу\n";
				continue;
			}
			if (choice > 5 || choice < 1) {
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
					for (int i = 0; i < students.size(); ++i) {
						cout << i + 1;
						students[i].printStudent();
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
				// if (res == 2) deleteTest(); // удаление теста
				if (res == 3) {
					cout << " ------- ТЕСТЫ В СИСТЕМЕ ------- " << endl;
					cout << string(45, '-') << endl;
					for (int i = 0; i < tests.size(); ++i) {
						cout << " [" << i + 1 << "]";
						tests[i].printTest();
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
				for (int i = 0; i < students.size(); ++i) {
					cout << i + 1;
					students[i].printStudent();
					cout << endl;
				}
				cout << string(56, '-') << endl << " Выберите номер студента: ";
				cin >> numSt;
				if (numSt > students.size()) {
					cout << "Некорректный ввод номера студента (1-" << students.size() << ")" << endl;
					break;
				}

				cout << "\n ------- ТЕСТЫ В СИСТЕМЕ ------- " << endl;
				cout << string(45, '-') << endl;
				for (int i = 0; i < tests.size(); ++i) {
					cout << " [" << i + 1 << "]";
					tests[i].printTest();
					cout << endl;
				}
				cout << string(45, '-') << endl << " Выберите номер теста: ";
				cin >> numTest;
				if (numTest > tests.size()) {
					cout << "Некорректный ввод номера теста (1-" << tests.size() << ")" << endl;
					break;
				}

				cout << " Введите результат прохождения теста (целое число): ";
				cin >> res;
				if (res > tests[numTest - 1].getScore()) {
					cout << "Некорректный ввод балла (1-" << tests[numTest - 1].getScore() << ")" << endl;
					break;
				}

				TestResult result(students[numSt - 1].getName(), tests[numTest - 1].getNameTest(), tests[numTest - 1].getScore(), res);
				results.push_back(result);
				cout << " Тест [" << tests[numTest - 1].getNameTest() << "] успешно пройден от лица студента[" <<
					students[numSt - 1].getName() << "] на балл = " << res << endl;
				break;
			}
			case 4: {
				cout << "\n == РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЙ ==\n";
				cout << string(50, '-') << endl;
				list<TestResult>::iterator it = results.begin();
				int ind = 1;
				for (it; it != results.end(); ++it) {
					cout << "---> [" << ind++ << "]" << endl;
					it->printTestResult();
				}
				cout << string(50, '-') << endl;
				break;
			}
			case 5:
				cout << "\n ВЫХОД ИЗ ПРОГРАММЫ...\n";
				flag = 0;
				break;
			}
		}
	}
};

