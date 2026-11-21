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
#include <queue>
#include <stack>
#include "TestResult.h"
#include "FileManager.h"
using namespace std;


struct StudentWithAvg {
	Student st;
	double avg;
};

struct ComparatorAvg {
	bool operator()(const StudentWithAvg& a, const StudentWithAvg& b) const {
		return a.avg > b.avg;
	}
};


class TestingManager
{
	unordered_map<string, Student> students;
	map<string, Test> tests;
	multimap<string, TestResult> results;
	queue<TestResult> queueResults;
	priority_queue<StudentWithAvg, vector<StudentWithAvg>, ComparatorAvg> badStudents;


public:
	TestingManager() {
		int loading = FileManager::loadData(students, tests, results, queueResults);
		if (loading == -1) {
			cout << "Дальнейшая работа невозможна, устраните ошибки с файлами\n";
			throw runtime_error("Ошибка работы с файлом");
		}
		else {
			loadPrQueue(results, badStudents);
			cout << "Данные с текстовых файлов успешно загружены\n";
		}
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

	void showTests() {
		cout << "\n ------- ТЕСТЫ В СИСТЕМЕ ------- " << endl;
		cout << string(45, '-') << endl;
		int ind = 1;
		map<string, Test>::iterator t = tests.begin();
		for (t; t != tests.end(); ++t) {
			cout << " [" << ind++ << "]";
			t->second.printTest();
			cout << endl;
		}
	}

	void showStudents() {
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
		system("pause");
	}

	void showResults() {
		cout << "\n == РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЙ ==\n";
		cout << string(50, '-') << endl;
		multimap<string, TestResult>::iterator r = results.begin();
		int ind = 1;
		for (r; r != results.end(); ++r) {
			cout << "---> [" << ind++ << "]" << endl;
			r->second.printTestResult();
			cout << endl;
		}
		cout << string(50, '-') << endl;
		system("pause");
	}

	void searchStbyResult() {
		system("cls");
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
			system("pause");
			return;
		}

		t = tests.begin();
		auto curTest = next(t, numTest - 1);
		string id = curTest->first;
		bool flag = 0;
		cout << "\n Студенты, которые прошли Тест [" << curTest->second.getTitle() << "] :" << endl;
		unordered_map<string, Student>::iterator curSt = students.begin();
		for (curSt; curSt != students.end(); ++curSt) {
			if (curSt->second.getDoneTests().count(id) > 0) {
				cout << " - " << curSt->second.getName() << endl;
				flag = true;
			}
		}
		if (!flag) cout << "Все студенты НЕ прошли этот Тест" << endl;
		system("pause");
		return;
	}

	void showBadStudents() {
		cout << " --- СТУДЕНТЫ , КОТОРЫЕ ПРИДУТ НА ПЕРЕСДАЧУ--- " << endl;
		cout << string(56, '-') << endl;
		cout << "№ | " << setw(6) << "Номер" << " | " << setw(30) << "ФИО" << " | " << setw(8) << "Группа" << " | " << endl;
		int ind = 0;
		auto tempQueue = badStudents;
		while (!tempQueue.empty()) {
			StudentWithAvg current = tempQueue.top();
			tempQueue.pop();

			if (current.avg <= 4.0) {
				cout << ++ind ;
				current.st.printStudent();
				cout << " (ср. балл: " << fixed << setprecision(2) << current.avg << ")" << endl;
			}
		}
		if (ind == 0) cout << " Cтудентов с пересдачей нет" << endl;
		else 	cout << string(56, '-') << endl;
		system("pause");
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
		system("pause");
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
			system("pause");
			return;
		}
		unordered_map<string, Student>::iterator it = students.begin();
		unordered_map<string, Student>::iterator itName = next(it, ch - 1);
		string nameSt = itName->second.getName();
		string idSt = itName->first;
		students.erase(itName);

		multimap<string, TestResult>::iterator r = results.begin();
		for (r; r != results.end();)
			if (idSt == r->first)
				r = results.erase(r);
			else r++;

		cout << " Cтудент [" << nameSt << "] успешно удален\n";
		system("pause");
	}

	void loadPrQueue(multimap<string, TestResult>& res, priority_queue<StudentWithAvg, vector<StudentWithAvg>, ComparatorAvg>& badSt) {
		map<string, vector<int>> scoresMap;

		while (!badSt.empty()) {
			badSt.pop();
		}

		for (const auto& test : res) {
			const auto& result = test.second; // получили результат из карты
			scoresMap[result.getStudent()].push_back(result.getCurRes());
		}

		for (auto sc = scoresMap.begin(); sc != scoresMap.end(); ++sc) {
			if (sc->second.empty()) continue;

			int sum = 0;
			for (int i = 0; i < static_cast<int>(sc->second.size()); ++i)
				sum += sc->second[i];
			double aver = static_cast<double>(sum) / static_cast<double>(sc->second.size());

			auto stIt = students.begin();
			for (stIt; stIt != students.end(); ++stIt) {
				if (stIt->second.getName() == sc->first) {
					break;
				}
			}
			if (stIt != students.end())
				badSt.push({ stIt->second, aver });
		}
	}

	void addTest() {
		string name, id;
		cout << string(15, '-') << endl;
		cin.ignore();
		cout << " Название теста: " << endl;
		getline(cin, name);
		cout << " Необходимо ввести [4] вопроса" << endl;
		cout << string(15, '-') << endl;

		string text;
		vector<Question> newQuestion;

		for (int t = 0; t < 4; ++t) {
			cout << " Введите текст вопроса: ";
			getline(cin, text);
			cin.ignore();

			array<string, 4> opt;
			int right;
			for (int i = 0; i < 4; ++i) {
				cout << " Вариант ответа [" << i + 1 << "]: ";
				getline(cin, opt[i]);
			}
			cout << " Номер правильного ответа (1-4): ";
			cin >> right;
			cin.ignore();
			newQuestion.push_back(Question(t + 1, text, opt, right));
		}

		string newId = Test::generateId();
		while (true) {
			if (tests.find(newId) == tests.end()) break;
			newId += 1;
		}
        Test newTest(Test::generateId(), name, newQuestion);
		tests.emplace(id, newTest);
		cout << "\n Тест [" << name << "] успешно создан!" << endl;
		system("pause");
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
			system("pause");
			return;
		}

		t = tests.begin();
		map<string, Test>::iterator delT = next(t, numTest - 1);
		string delName = delT->second.getTitle();
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
			if (doneTests.erase(delId) > 0) {
				s->second.addFullDoneTest(doneTests);
			}
		}
		cout << "Тест [" << delName << "] успешно удален из системы" << endl;
		system("pause");
	}

	void manageTests() {
			cout << "\n == УПРАВЛЕНИЕ ТЕСТАМИ ==\n";
			int res = insideMenu();
			if (res == -1) return;
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
			system("pause");
	}

	void checkResults() {
		system("cls");
		int com;
		while (true) {
			cout << "\n == ПРОВЕРКА ТЕСТОВ ==\n";
			cout << "\n 1) проверить тест и занести в журнал\n" <<
				" 0) выйти назад\n";
			cout << " Выбор: ";
			cin >> com;
			if (cin.fail()) {
				cout << "Возникла ошибка" << endl;
				cin.clear();
				cin.ignore(100, '\n');
				continue;
			}
			else if (com == 0) break;
			else if (com == 1) {
				if (queueResults.empty()) {
					cout << " Нет результатов тестов для проверки" << endl;
					system("pause");
					break;
				}
				const TestResult& currentRes = queueResults.front();
				currentRes.printTestResult();

				auto st = students.begin();
				for (; st != students.end(); ++st) {
					if (st->second.getName() == currentRes.getStudent()) {
						break;
					}
				}

				if (st != students.end()) {
					const string& idSt = st->first;
					bool found = false;

					for (auto it = results.begin(); it != results.end(); ++it) {
						if (it->second.getTest() == currentRes.getTest() && it->second.getStatus() == 0 && it->second.getStudent() == st->second.getName()) {
							it->second.setStatus(1);
							found = true;
							break;
						}
					}

					if (found) {
						loadPrQueue(results, badStudents);
						cout << " Результат проверен \n";
					}
				}
				else {
					cout << "Некорректный ввод (1 или 0)" << endl;
					continue;
				}
			}
		}
	}

	void checkResultsOfSt() {
		system("cls");
		int num;
		cout << "\n == ПРОСМОТР РЕЗУЛЬТАТОВ КОНКРЕТНОГО СТУДЕНТА  ==\n\n";
		showStudents();
		while (true) {
			cout << "\n | Введите № студента: ";
			cin >> num;
			if (cin.fail() || num > students.size() || num < 1) {
				cout << "Возникла ошибка" << endl;
				cin.clear();
				cin.ignore(100, '\n');
				continue;
			}
			break;
		}

		auto st = students.begin();
		for (int i =0; i < num -1; ++i)
			st++;
		Student temp = st->second;

		if (temp.getDoneTests().empty()) {
			cout << " Студент '" << temp.getName() << "' не проходил тесты" << endl;
			system("pause");
		}
		else {
			cout << " Результаты студента '" << temp.getName() << "':" << endl;
			for (auto res : results)
				if (res.first == temp.getId()) {
					res.second.printTestResult();
					cout << endl;
				}
			system("pause");
		}
		return;
	}

	void runningTest() {
			system("cls");
			cout << "\n == ПРОХОЖДЕНИЕ ТЕСТИРОВАНИЯ ==\n\n";
			int numSt, numTest;

			showStudents();
			while (true) {
				cout << "\n Выберите номер студента: ";
				cin >> numSt;
				if (numSt > students.size() || numSt < 1 || cin.fail()) {
					cout << "Ошибка: Некорректный ввод номера студента (1-" << students.size() << ")" << endl;
					cin.clear();
					cin.ignore(100, '\n');
					continue;
				}
				break;
			}
			unordered_map<string, Student>::iterator it = students.begin();
			unordered_map<string, Student>::iterator newSt = next(it, numSt - 1);

			showTests();
			while (true) {
				cout << "\n Выберите номер теста: ";
				cin >> numTest;
				if (numTest > tests.size() || numTest < 1 || cin.fail()) {
					cout << "Некорректный ввод номера теста (1-" << tests.size() << ")" << endl;
					cin.clear();
					cin.ignore(100, '\n');
					continue;
				}
				break;
			}
			map<string, Test>::iterator t = tests.begin();
			auto newT = next(t, numTest - 1);

			stack<int> stackQuestions;
			auto questions = newT->second.getQuestions();
			vector<Question>::iterator current = questions.begin();
			vector<int> answers(questions.size(), 0);

			while (current != questions.end()) {
				system("cls");
				cout << "\n == ТЕСТ '" << newT->second.getTitle() << "' == \n\n";
				cout << " - ВОПРОС " << current->getId() << " из 4 -\n";
				current->printQuestion();
				if (answers[current->getId()-1] != 0) {
					cout << "\n | Ваш сохранённый ответ = " << answers[current->getId() - 1] << endl;
				}
				cout << "\n 1 - Ответить и перейти к следующему вопросу\n" <<
					" 2 - Вернуться к прошлому вопросу\n" <<
					" 3 - Закончить тест\n";

				int com, answer;
				while (true) {
					cout << " | Команда: ";
					cin >> com;
					if (com > 3 || com < 1 || cin.fail()) {
						cout << "Некорректный ввод номера (1-3)" << endl;
						cin.clear();
						cin.ignore(100, '\n');
						continue;
					}
					break;
				}
				if (com == 1) {
					cout << "\n | Введите № ответа [1-4]: ";
					cin >> answer;
					if (answer > 4 || answer < 1 || cin.fail()) {
						cout << "Некорректный ввод " << endl;
						cin.clear();
						cin.ignore(100, '\n');
						continue;
					}
					answers[current->getId() - 1] = answer;
					stackQuestions.push(static_cast<int>(current->getId()));
					current++;
					if (current == questions.end()) {
						cout << " Это был последний вопрос. Тест завершается" << endl;
						system("pause");
						break;
					}
				}
				else
					if (com == 2) {
						if (current->getId() == 1 || stackQuestions.empty()) {
							cout << " Возврат к прошлому вопросу невозможен, так как это самый первый вопрос теста...\n";
							system("pause");
						}
						else {
							current--;
							stackQuestions.pop();
							cout << " Возврат к прошлому вопросу...\n";
							system("pause");
						}
					}
					else if (com == 3) {
						cout << " Вы завершаете прохождение теста...\n";
						system("pause");
						break;
					}
			}

			double resTest = 0;
			int id = 0;
			for (auto ans : answers) {
				if (answers[id] == questions[id].getCorrectNum() && answers[id] != 0)
					resTest += 2.5;
				id++;
			}

			newSt->second.addDoneTest(newT->second.getIdTest()); //сохранение теста в set студента
			TestResult result(newSt->second.getName(), newT->second.getTitle(), resTest);
			queueResults.push(result);
			results.emplace(newSt->first, result);
			cout << "\n Тест [" << newT->second.getTitle() << "] успешно пройден от лица студента[" <<
				newSt->second.getName() << "] на балл = " << setprecision(1) << resTest << " / 10" << 
				"\n Результат отправлен в очередь на проверку\n";
			system("pause");
	}

	void run() {
		int choice;
		int role = 0; // 0 - начальное меню, 1 - препод, 2 - студент
		while (true)  {
			if (role == 0) {
				system("cls");
				cout << "\n === Учет проведения тестирования по программированию среди студентов ===\n";
				cout << "\n --- НАЧАЛЬНОЕ МЕНЮ ---\n";
				cout << " | Выберите роль: \n" <<
					"1) Преподаватель;\n" <<
					"2) Студент;\n" <<
					"0) Выход;\n" <<
					" Выбор: ";

				cin >> choice;
				if (!cin.good()) {
					cout << "Возникла проблема с потоком ввода" << endl;
					cin.clear();
					cin.ignore(100, '\n');
					continue;
				}
				if (choice > 2 || choice < 0) {
					cout << "Некорректный ввод числового значения (1 или 2)" << endl;
					continue;
				}

				if (choice == 0) { cout << "Выход..." << endl; return; }
				else  role = choice;
			}
			else if (role == 1) {
				while (true && role == 1) {
					system("cls");
					Teacher::showMenuTeacher();

					int ch;
					while (true) {
						cout << " Выбор: ";
						cin >> ch;
						if (cin.fail()) {
							cout << "Возникла ошибка" << endl;
							cin.clear();
							cin.ignore(100, '\n');
							continue;
						}
						else
							if (ch > 6 || ch < 0) {
								cout << "Некорректный ввод команды меню" << endl;
								continue;
							}
							else break;
					}

					switch (ch) {
					case 0: role = 0; break;
					case 1: {
						int res = insideMenu();
						if (res == -1) break;
						if (res == 1)  addStudent();
						if (res == 2) deleteStudent(); // удаление студента
						if (res == 3) showStudents();
						break;
					}
					case 2: manageTests(); break;
					case 3: checkResults(); break;
					case 4: showResults(); break;
					case 5: searchStbyResult(); break;
					case 6: loadPrQueue(results, badStudents); showBadStudents(); break;
					}
				}
			}
			else if (role == 2 ) {
				while (true && role == 2) {
					system("cls");
					Student::showMenuStudent();

					int ch;
					while (true) {
						cout << " Выбор: ";
						cin >> ch;
						if (cin.fail()) {
							cout << "Возникла ошибка" << endl;
							cin.clear();
							cin.ignore(100, '\n');
							continue;
						}
						else
							if (ch > 3 || ch < 0) {
								cout << "Некорректный ввод команды меню" << endl;
								continue;
							}
							else break;
					}

					switch (ch) {
					case 0: role = 0; break;
					case 1:  showTests(); system("pause"); break;
					case 2: runningTest(); break;
					case 3: checkResultsOfSt(); break;
					}
				}
			}
		}
	}
};


