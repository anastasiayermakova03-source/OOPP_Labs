/*Реализовать класс String для работы со строками. Создать методы
доступа к полям класса.Перегрузить операторы « = »(присваивание
значения), « += »(сложение строки и объекта другого класса).Реализовать
конструкторы(по умолчанию, с параметрами, копирования), деструктор.
Реализовать friend - функции для операторов ввода / вывода в поток для созданного класса. */

#include "String.h"
#include <iostream>
#include <windows.h>
#include <cstring>
using namespace std;

int main_menu() {
    while (true) {
        int choice;
        cout << "\n=== MENU ===" << endl;
        cout << "| 1. Работать\n" <<
            "| 0. Выход\n";
        cout << "\tВаш выбор: ";
        cin >> choice;
        if (choice >= 0 && choice < 2)
            return choice;
        cout << "! Некорректный ввод !" << endl;
    }

}

int menu() {
    while (true) {
        int choice;
        cout << "\n=== MENU ===" << endl;
        cout << "| 1. Перегруженный оператор =\n" <<
            "| 2. Перегруженный оператор =+\n" << 
        "| 3. Выход" << endl;
        cout << "\tВаш выбор: ";
        cin >> choice;
        if (choice > 0 && choice < 4)
            return choice;
        cout << "! Некорректный ввод !" << endl;
    }
}

void input(const String& one, const String& two) {
    cout << "\nПервая строка - ";
    cout << one.Str();
    cout << "\nЕе размер = ";
    cout << one.SizeStr();
    cout << "\nВторая строка - ";
    cout << two.Str();
    cout << "\nЕе размер = ";
    cout << two.SizeStr();
    cout << endl;
    return;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    while (true) {
        int comanda = main_menu();
        switch (comanda) {
        case 1:
        {
            String s1, s2;
            cout << "| Введите первую строку: ";
            cin >> s1;
            cout << "| Введите вторую строку: ";
            cin >> s2;
            input(s1, s2);
            while (true) {
                int choice = menu();
                switch (choice) {
                case 1:
                    s1 = s2;    // используем перегруженный оператор =
                    input(s1, s2);
                    break;
                case 2:
                    s1 += s2;
                    input(s1, s2);
                    break;
                case 3:
                    cout << "Выход из программы...";
                    return 0;
                }
            }
            break;
        }
        case 0: cout << "Выход" << endl;
            return 0;
        default: cout << " Некорректный ввод" << endl;
            break;
        }
    }
}
