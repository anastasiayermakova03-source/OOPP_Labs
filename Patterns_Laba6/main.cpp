#include <array>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "AnswerChecker.h"
#include "TestQuestionCreator.h"
#include "QuestionHint.h"
#include "Student.h"
#include "Test.h"
#include "Testing.h"
#include "TestModeFactory.h"
#include "TestQuestion.h"
#include "TestResult.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");

	Testing app;
	app.load();

	while (true) {
		cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
		cout << "1) Просмотреть тесты\n";
		cout << "2) Просмотреть студентов\n";
		cout << "3) Просмотреть результаты\n";
		cout << "4) Пройти тестирование\n";
		cout << "5) Управление тестами\n";
		cout << "6) Управление студентами\n";
		cout << "7) Выход из системы\n";
		cout << "Выберите пункт: ";

		int menuChoice = 0;
		if (!(cin >> menuChoice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Ошибка ввода.\n";
			continue;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (menuChoice) {
		case 1: {
			cout << "\n=== СПИСОК ТЕСТОВ ===\n";
			if (!app.hasTests()) {
				cout << "Тестов пока нет.\n";
			}
			else {
				app.printTests();
			}
			break;
		}
		case 2: {
			cout << "\n=== СПИСОК СТУДЕНТОВ ===\n";
			if (!app.hasStudents()) {
				cout << "Студентов пока нет.\n";
			}
			else {
				app.printStudents();
			}
			break;
		}
		case 3: {
			cout << "\n=== РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ ===\n";
			if (!app.hasResults()) {
				cout << "Результатов пока нет.\n";
			}
			else {
				app.printResults();
			}
			break;
		}
		case 4: {
			if (!app.hasStudents()) {
				cout << "Нет ни одного студента.\n";
				break;
			}
			if (!app.hasTests()) {
				cout << "Нет ни одного теста.\n";
				break;
			}

			int modeChoice = 0;
			while (true) {
				cout << "\n=== ВЫБОР РЕЖИМА ПРОХОЖДЕНИЯ ===\n";
				cout << "1) Экзамен\n";
				cout << "2) Тренировка\n";
				cout << "Выберите режим: ";
				if (!(cin >> modeChoice)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Ошибка ввода.\n";
					continue;
				}
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				if (modeChoice == 1 || modeChoice == 2) {
					break;
				}
				cout << "Выберите 1 или 2.\n";
			}

			unique_ptr<TestModeFactory> modeFactory;
			if (modeChoice == 1) {
				modeFactory = make_unique<ExamModeFactory>();
			}
			else {
				modeFactory = make_unique<PracticeModeFactory>();
			}

			string stId;
			const Student* selectedStudent = nullptr;
			while (true) {
				cout << "\n=== ВЫБОР СТУДЕНТА ===\n";
				app.printStudents();
				cout << "Введите ID студента: ";
				getline(cin, stId);
				if (stId.find_first_not_of(" \t\r\n") == string::npos) {
					cout << "ID не может быть пустым.\n";
					continue;
				}
				selectedStudent = app.findStudent(stId);
				if (selectedStudent != nullptr) {
					break;
				}
				cout << "Студент с таким ID не найден.\n";
			}

			string testId;
			const Test* selectedTest = nullptr;
			while (true) {
				cout << "\n=== ВЫБОР ТЕСТА ===\n";
				app.printTests();
				cout << "Введите ID теста: ";
				getline(cin, testId);
				if (testId.find_first_not_of(" \t\r\n") == string::npos) {
					cout << "ID не может быть пустым.\n";
					continue;
				}
				selectedTest = app.findTest(testId);
				if (selectedTest != nullptr) {
					break;
				}
				cout << "Тест с таким ID не найден.\n";
			}

			unique_ptr<QuestionHint> hint = modeFactory->createHint();
			unique_ptr<AnswerChecker> checker = modeFactory->createChecker();

			int earnedScore = 0;
			const auto& questions = selectedTest->getTestQuestions();

			cout << "\n--- Начало теста \"" << selectedTest->getTitle() << "\" (" << modeFactory->getModeName() << ") ---\n";
			for (size_t i = 0; i < questions.size(); ++i) {
				const TestQuestion* q = questions[i];
				if (!q) {
					continue;
				}

				cout << "\nВопрос " << (i + 1) << " из " << questions.size() << ":\n";
				q->printTestQuestion();

				string hintText;
				if (hint && hint->getHint(*q, hintText)) {
					cout << hintText << "\n";
				}

				string userAnswer;
				while (true) {
					cout << "Ваш ответ: ";
					getline(cin, userAnswer);
					if (userAnswer.find_first_not_of(" \t\r\n") != string::npos) {
						break;
					}
					cout << "Ответ не может быть пустым.\n";
				}

				int pts = checker ? checker->checkAnswer(*q, userAnswer) : 0;
				earnedScore += pts;
				cout << "Начислено баллов: " << pts << "\n";
			}

			cout << "\nИтоговый балл: " << earnedScore << "\n";
			if (app.addResult(selectedStudent->getId(), selectedTest->getIdTest(), earnedScore)) {
				cout << "Результат сохранён.\n";
			}
			break;
		}
		case 5: {
			while (true) {
				cout << "\n=== УПРАВЛЕНИЕ ТЕСТАМИ ===\n";
				cout << "1) Создать новый тест\n";
				cout << "2) Удалить тест\n";
				cout << "0) Назад\n";
				cout << "Выберите пункт: ";

				int tChoice = 0;
				if (!(cin >> tChoice)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Ошибка ввода.\n";
					continue;
				}
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				if (tChoice == 0) {
					break;
				}
				if (tChoice == 1) {
					string id;
					while (true) {
						cout << "Введите ID теста: ";
						getline(cin, id);
						if (id.find_first_not_of(" \t\r\n") != string::npos) {
							break;
						}
						cout << "ID не может быть пустым.\n";
					}

					string title;
					while (true) {
						cout << "Введите название теста: ";
						getline(cin, title);
						if (title.find_first_not_of(" \t\r\n") != string::npos) {
							break;
						}
						cout << "Название не может быть пустым.\n";
					}

					int qCount = 5;
					while (true) {

						vector<TestQuestion*> questions;
						questions.reserve(static_cast<size_t>(qCount));

						for (int i = 0; i < qCount; ++i) {
							cout << "\n--- Вопрос " << (i + 1) << " из " << qCount << " ---\n";
							int qType = 0;
							while (true) {
								cout << "1) Вопрос с вариантами ответа\n2) Вопрос с ответом-определением\nТип вопроса: ";
								if (!(cin >> qType)) {
									cin.clear();
									cin.ignore(numeric_limits<streamsize>::max(), '\n');
									cout << "Ошибка ввода.\n";
									continue;
								}
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
								if (qType == 1 || qType == 2) {
									break;
								}
								cout << "Выберите 1 или 2:\n";
							}
							unique_ptr<TestQuestionCreator> creator;
							if (qType == 1) {
								creator = make_unique<ChoiceQuestionCreator>();
							}
							else {
								creator = make_unique<DefQuestionCreator>();
							}

							unique_ptr<TestQuestion> qPtr = creator->createQuestion();

							string qText;
							while (true) {
								cout << "Текст вопроса: ";
								getline(cin, qText);
								if (qText.find_first_not_of(" \t\r\n") != string::npos) {
									break;
								}
								cout << "Текст не может быть пустым.\n";
							}
							qPtr->setText(qText);

							if (auto* choiceQ = dynamic_cast<ChoiceQuestion*>(qPtr.get())) {
								array<string, 4> opts;
								for (int j = 0; j < 4; ++j) {
									while (true) {
										cout << "Вариант " << (j + 1) << ": ";
										getline(cin, opts[j]);
										if (opts[j].find_first_not_of(" \t\r\n") != string::npos) {
											break;
										}
										cout << "Вариант не может быть пустым.\n";
									}
								}
								int rightNum = 0;
								while (true) {
									cout << "Номер правильного варианта (1-4): ";
									if (!(cin >> rightNum)) {
										cin.clear();
										cin.ignore(numeric_limits<streamsize>::max(), '\n');
										cout << "Ошибка ввода.\n";
										continue;
									}
									cin.ignore(numeric_limits<streamsize>::max(), '\n');
									if (rightNum >= 1 && rightNum <= 4) {
										break;
									}
									cout << "Введите число от 1 до 4.\n";
								}
								choiceQ->setOptions(opts);
								choiceQ->setNumRight(rightNum);
							}
							else if (auto* defQ = dynamic_cast<DefQuestion*>(qPtr.get())) {
								string ans;
								while (true) {
									cout << "Правильный ответ: ";
									getline(cin, ans);
									if (ans.find_first_not_of(" \t\r\n") != string::npos) {
										break;
									}
									cout << "Ответ не может быть пустым.\n";
								}
								defQ->setCorAnswer(ans);
							}

							questions.push_back(qPtr.get());
						}

						Test newTest(id, title, questions);
						for (TestQuestion* q : questions) {
							delete q;
						}

						if (app.addTest(newTest)) {
							cout << "Тест создан.\n";
						}
						else {
							cout << "Тест с таким ID уже существует.\n";
						}
					}
				}
				else if (tChoice == 2) {
					if (!app.hasTests()) {
						cout << "Список тестов пуст.\n";
						continue;
					}
					app.printTests();
					string delId;
					cout << "Введите ID теста для удаления (пустая строка — отмена): ";
					getline(cin, delId);
					if (delId.find_first_not_of(" \t\r\n") == string::npos) {
						continue;
					}
					if (app.removeTest(delId)) {
						cout << "Тест удалён.\n";
					}
					else {
						cout << "Тест с таким ID не найден.\n";
					}
				}
				else {
					cout << "Неверный пункт.\n";
				}
			}
			break;
		}
		case 6: {
			while (true) {
				cout << "\n=== УПРАВЛЕНИЕ СТУДЕНТАМИ ===\n";
				cout << "1) Добавить студента\n";
				cout << "2) Удалить студента\n";
				cout << "0) Назад\n";
				cout << "Выберите пункт: ";

				int sChoice = 0;
				if (!(cin >> sChoice)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Ошибка ввода.\n";
					continue;
				}
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				if (sChoice == 0) {
					break;
				}
				if (sChoice == 1) {
					string id;
					while (true) {
						cout << "Введите ID студента: ";
						getline(cin, id);
						if (id.find_first_not_of(" \t\r\n") != string::npos) {
							break;
						}
						cout << "ID не может быть пустым.\n";
					}

					string name;
					while (true) {
						cout << "Введите ФИО студента: ";
						getline(cin, name);
						if (name.find_first_not_of(" \t\r\n") != string::npos) {
							break;
						}
						cout << "Имя не может быть пустым.\n";
					}
					string gr;
					while (true) {
						cout << "Введите группу студента: ";
						getline(cin, gr);
						if (gr.find_first_not_of(" \t\r\n") != string::npos) {
							break;
						}
						cout << "Имя не может быть пустым.\n";
					}

					if (app.addStudent(Student(id, name, stoi(gr)))) {
						cout << "Студент добавлен.\n";
					}
					else {
						cout << "Студент с таким ID уже существует.\n";
					}
				}
				else if (sChoice == 2) {
					if (!app.hasStudents()) {
						cout << "Список студентов пуст.\n";
						continue;
					}
					app.printStudents();
					string delId;
					cout << "Введите ID студента для удаления (пустая строка — отмена): ";
					getline(cin, delId);
					if (delId.find_first_not_of(" \t\r\n") == string::npos) {
						continue;
					}
					if (app.removeStudent(delId)) {
						cout << "Студент удалён.\n";
					}
					else {
						cout << "Студент с таким ID не найден.\n";
					}
				}
				else {
					cout << "Неверный пункт.\n";
				}
			}
			break;
		}
		case 7:
			app.save();
			cout << "Выход из системы.\n";
			return 0;
		default:
			cout << "Неверный пункт меню.\n";
			break;
		}
	}
}