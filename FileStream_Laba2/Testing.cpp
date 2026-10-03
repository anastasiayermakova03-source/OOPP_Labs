/* Разработать набор классов (минимум 5 классов, связи между классами: агрегация, композиция, наследование)
по предметной области «Тестирование программного обеспечения». Функционал программы
должен позволить создать перечень тестов для выполнения тестирования программного обеспечения. */

#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h>
#include <fstream>
#include "Test.h"
#include "TestProcess.h"
using namespace std;

int menu() {
    cout << "\n ---- МЕНЮ ----- " << endl;
    cout << "1) просмотр каталога тестов\n" <<
        "2) просмотр каталога готовых наборов тестов\n" <<
        "3) добавить новый тест\n" <<
        "4) создать новый набор тестов\n" <<
        "5) удалить набор тестов\n" <<
        "6) закончить\n";
    int choice_menu;
    while (true) {
        cout << "ВЫБЕРИТЕ: ";
        cin >> choice_menu;
        if (cin.good()) {
            if (choice_menu > 6 || choice_menu < 1)
                cout << "Команда [" << choice_menu << "] отсутствует" << endl;
            else
                break;
        }
        cin.clear();
        cout << "Некорректный ввод" << endl;
        cin.ignore(100, '\n');
    }
    return  choice_menu;
}

template<typename T>
void print(T** t, int count) {
    if (t == nullptr || count <= 0) {
        cout << "Ошибка табличного вывода" << endl;
        return;
    }

    cout << "\n " << string(97, '-') << "\n";
    cout << " | " << setw(7) << "ID" << " | " << setw(25) << "Название" << " | " << setw(55) << "Описание" << " | \n";
    cout << " " << string(97, '+') << "\n";
    for (int i = 0; i < count; i++) {
        t[i]->print();
        cout << " " << string(97, '-') << "\n";
    }
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char buf[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buf);
    cout << "Рабочая папка: " << buf << endl;

    Test** tests = nullptr;
    TestCollection** testCollections = nullptr;
    FileManager fileManager;
    int count_tests = fileManager.loadTest(tests);
    int count_coll = fileManager.loadTestCollection(testCollections, tests, count_tests);

    cout << "\n ====== СОЗДАНИЕ ТЕСТОВ И НАБОРОВ ТЕСТОВ ====== \n";
    while (true) {
        switch (menu()) {
        case 1:
            cout << " ---- Просмотр каталога тестов ---- \n";
            print(tests, count_tests); break;
        case 2:
            cout << " ---- Просмотр каталога готовых наборов тестов ---- \n";
            print(testCollections, count_coll); break;
        case 3: {
            cout << " ---- Добавление теста ---- \n";
            string name, id, describe, type, excRes, maxTime;
            Status st = failure;

            cin.ignore(100, '\n');
            cout << " Тип теста ( F - функциональный , P - производительный ): ";
            getline(cin, type);
            if (type != "F" && type != "P") {
                cout << "Ошибка. Такого типа не существует\n";
                break;
            }
            cout << " Название: ";
            getline(cin, name);
            cout << " Краткое описание: ";
            getline(cin, describe);

            Test** tempTests = new Test * [count_tests + 1];
            for (int i = 0; i < count_tests; i++) {
                tempTests[i] = tests[i];
            }

            if (type == "F") {
                cout << " Ожидаемый результат: ";
                getline(cin, excRes);
                tempTests[count_tests] = new FunctionalTest("TC-" + to_string(count_tests), name, describe, st, excRes, excRes);
            }
            if (type == "P") {
                cout << " Max время прохождения: ";
                getline(cin, maxTime);
                tempTests[count_tests] = new PerformanceTest("PT-" + to_string(count_tests), name, describe, st, stoi(maxTime), stoi(maxTime));
            }
            delete[] tests; 
            tests = tempTests;
            count_tests++;

            fileManager.saveTest(tests, count_tests);
            cout << "Тест [" << name << "] добавлен в каталог\n" << endl;
            break;
        }
        case 4: {
            cout << " ---- Создание нового набора тестов ---- \n";
            string name;
            int num;
            char ch;

            cout << " Название набора: ";
            cin.ignore(100, '\n');
            getline(cin, name);

            TestCollection* newColl = new TestCollection();
            newColl->setNameTestCollection(name);

            while (true) {
                if (count_tests == 0) {
                    cout << "В каталоге нет доступных тестов. Сначала добавьте тесты в базу.\n";
                    delete newColl;
                    break;
                }

                cout << "\n ---- Каталог тестов ---- \n";
                print(tests, count_tests);
                cout << "\n Выберите тест, который вы хотите добавить в набор [" << name << "] : ";
                cin >> num;
                if (num < 1 || num > count_tests) {
                    cout << "Ошибка. Такого теста нет." << endl;
                    continue;
                }

                cout << " Добавить тест №" << num << " [" << tests[num - 1]->getName() << "] в набор? (y - добавить / n - закончить создание): ";
                cin >> ch;
                if (ch == 'y') {
                    newColl->addTest(tests[num - 1]);
                    cout << " Тест №" << num << " успешно добавлен в набор [" << name << "].\n";

                    char cont;
                    cout << " Продолжить добавление других тестов для набора [" << name << "]? (y/n): ";
                    cin >> cont;
                    if (cont == 'y') continue;
                    else {
                        if (cont == 'n') ch = 'n';
                        else {
                            cout << "Ошибка. Введен некорректный символ. Переход к сохранению набора...\n";
                            ch = 'n';
                        }
                    }
                }
                if (ch == 'n') {
                    if (newColl->getCount() == 0) {
                        cout << " В набор не был добавлен ни один тест. Набор [" << name << "] не сохранен.\n";
                        delete newColl;
                        break;
                    }

                    TestCollection** tempColl = new TestCollection * [count_coll + 1];
                    for (int i = 0; i < count_coll; i++) {
                        tempColl[i] = testCollections[i];
                    }
                    tempColl[count_coll] = newColl;

                    delete[] testCollections;
                    testCollections = tempColl;
                    count_coll++;
                    fileManager.saveTestCollection(testCollections, count_coll);
                    break;
                }
                if (ch != 'n' && ch != 'y') {
                    cout << "Ошибка. Введен некорректный символ. Набор [" << name << "] не будет сохранен" << endl;
                    break;
                }
            }break;
        }
        case 5: {
            cout << "\n ---- Удаление набора тестов ---- \n";
            cout << "\n ---- Каталог готовых наборов тестов ---- \n";
            print(testCollections, count_coll);

            int num;
            cout << " Введите номер набора теста, который вы хотите удалить: ";
            cin >> num;
            if (num < 1 || num > count_coll) {
                cout << "Ошибка. Набора с таким номером нет.\n";
                break;
            }
            fileManager.saveDeleteColl(testCollections, count_coll, num-1);
            count_coll = fileManager.loadTestCollection(testCollections, tests, count_tests);
            break;
        }
        case 6:
            fileManager.saveTest(tests, count_tests);
            fileManager.saveTestCollection(testCollections, count_coll);
            cout << "\n << Завершение программы >>" << endl;
            return 0;
        }
    }
}