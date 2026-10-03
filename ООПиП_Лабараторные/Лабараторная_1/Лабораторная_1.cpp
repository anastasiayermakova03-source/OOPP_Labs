/*6. Создать класс Employee(сотрудник), одно из полей которого хранит «порядковый номер»
созданного объекта, т.е.для первого созданного объекта значение
этого поля равно 1, для второго − 2 и т.д.В классе разработать метод, который для объекта 
будет выводить на экран его порядковый номер, 
например: «Мой порядковый номер : 2».В программу добавить
необходимый набор полей и методов(минимум два поля и два метода) на свое усмотрение. */

#include "Employee.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

void simbol(int n) {
    for (int i = 0; i < n; i++)
        cout << "-";
}

int menu() {
    int choice;
    simbol(15);
    cout << " МЕНЮ " ;
    simbol(15);
    cout << "\n";
    cout << "| 1. Добавить сотрудника\n" <<
        "| 2. Определить номер сотрудника по имени\n" <<
        "| 3. Вывести весь список сотрудников\n" <<
        "| 4. Загрузить список в файл\n" <<
        "| 5. Восстановить список из файла\n" <<
        "| 0. Выйти" << endl;
    simbol(36);
    cout << endl;
    cout << "\tВаш выбор: ";
    cin >> choice;
    cin.ignore();
    return choice;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "=== Программа учета сотрудников ===\n" << endl;
    int choice;
    Employee::startEmp();
    while (1) {
        choice = menu();
        string name;
        int number;
        switch (choice) {
        case 1:
            Employee::addEmp();
            break;
        case 2:
            Employee::searchNum();
            break;
        case 3:
            Employee::printEmp();
            break;
        case 4:
            Employee::saveToFile("employees.txt");
            break;
        case 5:
            Employee::loadFromFile("employees.txt");
            break;
        case 0: 
            Employee::deleteEmp();
            cout << "Выход из программы..." << endl;
            return 0;
        default:
            cout << "Некорректное значение!" << endl;
            break;
        }
    }
}
