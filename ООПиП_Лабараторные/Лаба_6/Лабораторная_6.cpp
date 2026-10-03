/*Построить иерархию классов согласно схеме наследования, приведенной на рисунке ниже, 
по любой предметной области. Каждый класс должен содержать необходимые конструкторы и методы 
работы с полями классов. Функция main() должна иллюстрировать работу с массивами
объектов всех созданных классов. При необходимости атрибуты доступа при наследовании можно изменять. 
При необходимости самостоятельно добавить классы для реализации множественного наследования. */

#include "Smartphone.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>
using namespace std;

int menu0() {
    int choice;
    while (true) {
        cout << "\n===== МЕНЮ нулевого уровня =====\n" <<
            " 1. Начать работу\n 2. Завершить программу\n" << "\tВаш выбор: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); 
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
            " 1. Добавить smartphone\n 2. Добавить gamephone\n" <<
            " 3. Просмотреть массив\n" << " 4. Удалить элемент\n" <<
            " 0. Выход из программы\n" << "\tВаш выбор: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(10000, '\n');
            cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
        }
        else if (choice < 0 || choice > 4)
            cout << "Такого варианта нет :(" << endl;
        else break;
    }
    return choice;
}

void Show(Smartphone* array[], int & size) {
    if (size == 0) {
        cout << "\nВ файле нет записей" << endl;
        return;
    }
    cout << "\n\t====== SMARTPHONE ======\n";
    for (int i = 0; i < size; i++) {
        cout << " ------ [" << i + 1 << "] ------\n";
        array[i]->print();
        cout << " -----------------\n";
        cout << endl;
    }
    return;
}


void Add(int t,Smartphone* array[], int & size) {
    ofstream Out("smartphones.txt", ios::app);
    if (!Out.is_open()) {
        cout << "Ошибка открытия файла" << endl;
        return;
    }
    else {
        cout << "\n====== ДОБАВЛЕНИЕ СМАРТФОНА ======\n";
        string model, OS, screen, stab;
        bool cooling;
        int core, resolution, memory, battery, clock;
        double diagonal;
        cout << "| Введите ОС: ";
        cin.ignore(1000, '\n');
        getline(cin, OS);
        cout << "| Введите название модели: ";
        getline(cin, model);
        cout << "| Введите время автономной работы (в часах): ";
        cin >> battery;
        cout << "| Введите размер ОП: ";
        cin >> memory;
        cout << "| Введите диагональ экрана: ";
        cin >> diagonal;
        cout << "| Введите разрешение: ";
        cin >> resolution;
        cin.ignore(1000, '\n');
        cout << "| Введите тип стабилизации у камеры: ";
        getline(cin, stab);
        cout << "| Введите кол-во ядер: ";
        cin >> core;
        cout << "| Введите тактовую частоту: ";
        cin >> clock;
        if (t == 2) {     // то есть игровой телефон
            cout << "| Введите, есть ли функция охлаждения (1 - есть; 0 - нет): ";
            cin >> cooling;
            cin.ignore(10000, '\n');
            cout << "| Введите тип экрана: ";
            getline(cin,screen);
            array[size] = new Gamephone(cooling, screen, OS, model, battery, memory, diagonal, resolution, stab, core, clock);
        }
        else 
            array[size] = new Smartphone(OS, model, battery, memory, diagonal, resolution, stab, core, clock);
        array[size++]->saveFile(Out);
        cout << "\n Успешно добавлен элемент номер [" << size << "]\n";
        Out.close();
        return;
    }
}


void loadFile(Smartphone* array[], int& size) {
    ifstream In("smartphones.txt");
    if (!In.is_open()) {
        cout << "Ошибка открытия файла" << endl;
        for (int i = 0; i < 5; i++)
            array[i] = new Smartphone();
    }
    else {
        string model, OS, screen, type, stab;
        bool cooling;
        int core, resolution, memory, battery, clock;
        double diagonal;
        for (int i = 0; In >> type; i++) {
            if (type == "Gamephone") {
                In >> OS >> model >> cooling >> screen >> battery >> memory >> diagonal >>
                    resolution >> stab >> core >> clock;
                array[i] = new Gamephone(cooling, screen, OS, model, battery, memory, diagonal, resolution, stab, core, clock);
                size++;
            }
            else {
                In >> OS >> model >> battery >> memory >> diagonal >> resolution >> stab >> core >> clock;
                array[i] = new Smartphone(OS, model, battery, memory, diagonal, resolution, stab, core, clock);
                size++;
            }
        }
        In.close();
        cout << "--> Файл загружен - " << size << endl;
    }
}


void Delete(Smartphone* array[], int& size) {
    if (size == 0) {
        cout << "Ошибка, список смартфонов пока пуст!" << endl;
        return;
    } 
    Show(array, size);
    int del;
    while (true) {
        cout << "\n| Ввдеите номер элемента, который вы хотите удалить:";
        cin >> del;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
        }
        else if (del < 1 || del > size)
            cout << " Такого варианта нет :(" << endl;
        else break;
    }

    ifstream In("smartphones.txt");
    ofstream Out("buffer.txt");
    if (!In.is_open() || !Out.is_open()) {
        cout << " Ошибка открытия файла";
        return;
    }
    string model, OS, type, screen, stab;
    bool cooling;
    int core, resolution, memory, battery, clock;
    double diagonal;
    for (int i = 1; i < size; i++) {
        In >> type;
        if (type == "Gamephone") {
         In >> cooling >> screen >> OS >> model >>  battery >> memory >> diagonal >> resolution >> stab >> core >> clock;
            if (i != del)
                Out << type << " " << cooling << " " << screen << " " << OS << " " << model << " " << 
                battery << " " << memory << " " << diagonal << " " << resolution << " " << stab << " " << 
                core << " " << clock << endl;
        }
        else if (type == "Smartphone") {
            In >> OS >> model >> battery >> memory >> diagonal >> resolution >> stab >> core >> clock;
            if (i != del)
                Out << type << " " << OS << " " << model << " " << battery << " " << memory << " " << 
                diagonal << " " << resolution << " " << stab << " " << core << " " << clock << endl;;
        }
    }
    In.close(); Out.close();
    remove("smartphones.txt"); 
    rename("buffer.txt", "smartphones.txt");
    cout << " Элемент [" << del << "] успешно удален" << endl;

    for (int i = 0; i < size; i++)
        delete array[i];
    size = 0;
    loadFile(array, size);
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    switch (menu0()) {
    case 2: cout << "\nВыход из программы...";
        return 0;
    case 1:
        int size = 0;
        Smartphone* array[30];
        loadFile(array, size);
        while(true)
            switch (menu()) {
            case 1: Add(1, array, size); break;
            case 2: Add(2, array, size); break;
            case 3: Show(array, size); break;
            case 4: Delete(array, size);  break;
            case 0: cout << "Выход из программы..." << endl;
                return 0;
            }
    }
}