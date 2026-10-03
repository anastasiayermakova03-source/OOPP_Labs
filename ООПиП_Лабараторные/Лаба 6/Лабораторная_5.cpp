/* Разработать связи между классами «Часы», «Механические часы», «Электронные часы»,
«Циферблат», «Источник энергии». Все классы должны содержать методы получения
и изменения всех полей. Написать программу, позволяющую получать сведения о часах.
Использовать конструктор с параметрами, конструктор без параметров, конструктор копирования. 
В класс добавить необходимый набор полей и методов (минимум два поля и два метода) на свое усмотрение.
Предусмотреть метод для записи полученных данных в файл. Выводить на экран информацию о
десяти часах, имеющих наибольшую стоимость.
*/

#include "Watch.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;

int menu0() {
    int choice;
    while (true) {
        cout << "\n===== МЕНЮ нулевого уровня =====\n" <<
            " 1. Начать работу\n 2. Завершить программу\n" << "\tВаш выбор: ";
        cin >> choice;
              if (cin.fail()) {
                    cin.clear(); // Сбрасываем флаг ошибки
                    cin.ignore(10000, '\n');
                    cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
              }
                else if (choice != 1 && choice != 2)
                    cout << "Такого варианта нет :(" << endl;
                else break;
    }
    return choice;
}

int menu() {
    int choice;
    while (true) {
        cout << "\n===== ОСНОВНОЕ МЕНЮ =====\n" <<
            " 1. Добавить\n 2. Вывести топ-5 самых дорогих часов\n" <<
            " 3. Просмотр часов\n" << " 4. Выход из программы\n" << "\tВаш выбор: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); // Сбрасываем флаг ошибки
            cin.ignore(10000, '\n');
            cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
        }
        else if (choice < 1 || choice > 4)
            cout << "Такого варианта нет :(" << endl;
        else break;
    }
    return choice;
}

void ShowWatches(MechWatch* mechWatches[], ElWatch* elWatches[], int& mechSize, int& elSize) {
    if (elSize == 0 && mechSize == 0) {
        cout << "\nВ файле нет часов" << endl;
        return;
    }
    cout << "\n\t=== ЧАСЫ ===\n";
    for (int i = 0; i < elSize; i++) {
        cout << "\t[" << i + 1 << "]";
        elWatches[i]->print();
        cout << endl;
    }
    for (int i = 0; i < mechSize; i++) {
        cout << "\t[" << i + elSize + 1 << "]";
        mechWatches[i]->print();
        cout << endl;
    }
}

void AddWatch(MechWatch* mechWatches[], ElWatch* elWatches[], Dial* array_d[], PowerSource* array_ps[], int& mechSize, int& elSize) {
    ofstream outEl("elWatches.txt", ios::app);
    ofstream outMech("mechWatches.txt", ios::app);
    if (!outMech.is_open() || !outEl.is_open()) {
        cout << "Ошибка открытия файла" << endl;
        return ;
    }
    else {
        cout << "\n=== ДОБАВЛЕНИЕ ЧАСОВ ===\n";
            int x;
            cout << "\nВыберите эл.(1) или мех.(2) часы: " << endl;
            cout << "Команда - ";
            cin >> x;
            string br;
            double price;
            int indD, indP;
            cout << "Введите бренд: ";
            cin.ignore(10000, '\n');
            getline(cin, br);
            cout << "Введите цену: ";
            cin >> price;
            cout << endl;
            for (int i = 0; i < 5; i++) { cout << "[" << i + 1 << "] ";  array_d[i]->print(); }
            cout << endl;
            cout << "Введите номер циферблата (1-5): ";
            cin >> indD;
            cout << endl;
            for (int i = 0; i < 5; i++) { cout << "[" << i + 1 << "] "; array_ps[i]->print(); }
            cout << endl;
            cout << "Введите номер источника (1-5): ";
            cin >> indP;
            indP--; indD--;
            int f1, f2;
            if (x == 1) {
                cout << "\nЕсть ли подсветка?  1)да  2)нет" << endl;
                cout << "Выбор: ";
                cin >> f1;
                cout << "\nЕсть ли smart функция?  1)да  2)нет" << endl;
                cout << "Выбор: ";
                cin >> f2;
                elWatches[elSize] = new ElWatch(br, price, array_d[indD], array_ps[indP], indD, indP, f1, f2);
                elWatches[elSize]->saveFile(outEl);
                elSize++;
            }
            else if (x == 2) {
                cout << "\nКол-во камней? ";
                cin >> f1;
                cout << "\nЕсть ли автоматический завод?  1)да  2)нет" << endl;
                cout << "Выбор: ";
                cin >> f2;
                mechWatches[mechSize] = new MechWatch(br, price, array_d[indD], array_ps[indP], indD, indP, f1, f2);
                mechWatches[mechSize]->saveFile(outMech);
                mechSize++;
            }
            else { cout << " ! Некорректный ввод !" << endl; return; }
        outEl.close();
        outMech.close();
        cout << " Часы добавлены в файл :)" << endl;
        return;
    }
}

void Top5Watches(MechWatch* mechWatches[], ElWatch* elWatches[], int& mechSize, int& elSize) {
    if (elSize == 0 && mechSize == 0) {
        cout << "Ошибка! Нет часов!" << endl;
        return;
    }

    int total = elSize + mechSize;
    Watch* watches[30];
    int current = 0;

    for (int i = 0; i < elSize; i++) {
        watches[current] = elWatches[i];
        current++;
    }
    for (int i = 0; i < mechSize; i++) {
        watches[current] = mechWatches[i];
        current++;
    }
    for(int i =0; i< current - 1; i++)
    for (int j = 0; j < current - i - 1; j++) {
          if (watches[j]->calculatePrice() < watches[j + 1]->calculatePrice()) {
              Watch* buffer = watches[j];
              watches[j] = watches[j + 1];
              watches[j + 1] = buffer;
          }
    }
    cout << "\n=== ТОП-5 САМЫХ ДОРОГИХ ЧАСОВ ===" << endl;
    int limit;
    if (current < 5) limit = current;
    else limit = 5;
    cout << "============================" << endl;
    for (int i = 0; i < limit; i++) {
        cout << "\t[" << i + 1 << "]";
        watches[i]->print();
        cout << endl;
    }
    cout << "============================" << endl;
}



int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    switch (menu0()) {
    case 2: cout << "\nВыход из программы...";
        return 0;
    case 1:
        int size = 0;
        Dial * array_d[5];
        ifstream in_d("dials.txt");
        if (!in_d.is_open()) {
            cout << "Ошибка открытия файла" << endl;
            for (int i = 0; i < 5; i++)
                array_d[i] = new Dial();
        }
        else {
            string m, c;
            int d;
            for (int i = 0; i < 5; ++i) {
                in_d >> m >> c >> d;
                array_d[i] = new Dial(m, c, d);
            }
            in_d.close();
            cout << "--> Циферблаты загружены из файла" << endl;
        }

        PowerSource* array_ps[5];
        ifstream in_p("powersources.txt");
        if (!in_p.is_open()) {
            cout << "Ошибка открытия файла" << endl;
            for (int i = 0; i < 5; i++)
                array_ps[i] = new PowerSource();
        }
        else {
            string t;
            int cap;
            for (int i = 0; i < 5; i++) {
                in_p >> t >> cap;
                array_ps[i] = new PowerSource(t, cap);
            }
            in_p.close();
            cout << "--> Источники энергии загружены из файла" << endl;
        }

        MechWatch *mechWatches[15];
        int mechSize = 0;
        ifstream inMech("mechWatches.txt");
        if (!inMech.is_open()) {
            cout << "Ошибка открытия файла" << endl;
        }
        else {
            string br;
            double pr;
            int f1, f2;
            int indD, indP;
            while (inMech >> br >> pr >> indD >> indP >> f1 >> f2)
                    mechWatches[mechSize++] = new MechWatch(br, pr, array_d[indD], array_ps[indP], indD, indP, f1, f2);
            inMech.close();
            cout << "--> Мех.часы загружены из файла - " << mechSize << endl;
        }

        ElWatch *elWatches[15];
        int elSize = 0;
        ifstream inEl("elWatches.txt");
        if (!inEl.is_open()) {
            cout << "Ошибка открытия файла" << endl;
        }
        else {
            string br;
            double pr;
            int f1, f2;
            int indD, indP;
            while (inEl >> br >> pr >> indD >> indP >> f1 >> f2)
                elWatches[elSize++] = new ElWatch(br, pr, array_d[indD], array_ps[indP], indD, indP, f1, f2);
            inEl.close();
            cout << "--> Эл.часы загружены из файла - " << elSize << endl;
        }

        while (true) {
            switch (menu()) {
            case 1: AddWatch(mechWatches, elWatches, array_d, array_ps, mechSize, elSize);
                break;
            case 2: Top5Watches(mechWatches, elWatches, mechSize, elSize);
                break;
            case 3: ShowWatches(mechWatches, elWatches, mechSize, elSize);
                break;
            case 4: cout << "\nВыход из программы..." << endl;

                for (int i = 0; i < 5; i++) {
                    delete array_d[i];
                    delete array_ps[i];
                }
                for (int i = 0; i < mechSize; ++i)
                    delete mechWatches[i];
                for (int i = 0; i < elSize; ++i)
                    delete elWatches[i];
                return 0;
            }
        }
        break;
    }
}
