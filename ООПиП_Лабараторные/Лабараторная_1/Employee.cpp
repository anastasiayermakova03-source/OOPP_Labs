#include "Employee.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include <fstream>

using namespace std;

int Employee::count = 0;
int Employee::capacity = 15;
Employee* Employee::massiv = nullptr;

Employee::Employee() : name("Без имени"), number(++count) {}

Employee::Employee(string n) : name(n), number(++count) {
    cout << "Создан сотрудник \"" << name << "\"\n" << endl;
}

Employee::Employee(const Employee& cp): name(cp.name), number(++count) {
    cout << "Cоздана копия \"" << name << "\"\n" << endl;
}

Employee::~Employee() {
    cout << "Удален сотрудник \"" << name << "\"\n" <<  endl;
}

string Employee::getName() const { return name; }
int Employee::getNumber() const { return number; }

void Employee::setName(const string& n) { name = n; };
void Employee::setNumber(const int& num) { number = num; };

void Employee::startEmp() {
    Employee::massiv = new Employee[capacity];
    count = 0;
}

void Employee::addEmp() {
    if (count == capacity) {
        cout << "Массив переполнен\n";
        int new_cap = capacity * 2;
        Employee* new_massiv = new Employee[new_cap];
        for (int i = 0; i < count; i++) 
            new_massiv[i] = Employee::massiv[i];

        delete[] massiv;
        massiv = new_massiv;
        capacity = new_cap;
        cout << "Поэтому объем массива увеличен: " << capacity << endl;
    }
    string n;
    cout << "| Введите имя сотрудника: ";
    getline(cin, n);
    count++;
    Employee::massiv[count-1].setName(n);
    Employee::massiv[count-1].setNumber(count);
    cout << "Создан сотрудник " << massiv[count-1].getNumber() << ".\"" << n << "\"\n" << endl;
}

void Employee::searchNum() {
    string n;
    cout << "| Введите имя сотрудника: ";
    getline(cin, n);
    int num = 0;
    for (int i = 0; i < count; i++) {
        if (n == massiv[i].getName()) {
            num = massiv[i].getNumber();
            cout << "Номер сотрудника с именем \"" << n << "\" : " << num <<  "\n" << endl;
            return;
        }
    }
        cout << "Сотрудника с именем \"" << n << "\" нет в списке" << "\n" << endl;
        return;
 }

void Employee::printEmp() {
    if (count == 0) {
        cout << "Список сотрудников пуст\n" << endl;
        return;
    }
    cout << "\n---- ВСЕ СОТРУДНИКИ ----" << endl;
    cout << setw(5) << "Номер" << "|" << setw(10) << "Имя" << endl;
    cout << "------------------------" << endl;
    for (int i = 0; i < count; i++)
        cout << setw(5) << massiv[i].getNumber() << "|" << setw(10) << massiv[i].getName() << endl;
    cout << "------------------------\n" << endl;
}

void Employee::deleteEmp() {
    if (count == 0) {
        cout << "Список сотрудников пуст" << endl;
        return;
    }
    delete[] Employee::massiv;
    Employee::massiv = nullptr;
    count = 0;
    capacity = 15;
    cout << "Все сотрудники удалены\n" << endl;
}


void Employee::saveToFile(const string& file) {
    ofstream fout(file);
    if (!fout.is_open()) { 
        cout << "Ошибка: не удалось открыть файл для записи\n"; 
        return; 
    } 
    for (int i = 0; i < count; i++) {
        fout << massiv[i].getNumber() << " " << massiv[i].getName() << "\n"; 
    } 
    fout.close();
    cout << "Данные сохранены в файл \"" << file << "\"\n"; 
}


void Employee::loadFromFile(const string& file) {
    ifstream fin(file);
    if (!fin.is_open()) {
        cout << "Файл не найден\n";
        return;
    }
    deleteEmp();
    int num;
    string name;
    while (fin >> num) {
        getline(fin, name);
        if (name.size() > 0 && name[0] == ' ')
            name.erase(0, 1);

        if (count == capacity) {
            int new_cap = capacity * 2;
            Employee* new_massiv = new Employee[new_cap];
            for (int i = 0; i < count; i++)
                new_massiv[i] = massiv[i];
            delete[] massiv;
            massiv = new_massiv;
            capacity = new_cap;
        }
        massiv[count].setNumber(num); //заносим в массив то, что прочитали из файла
        massiv[count].setName(name); count++;
    } 
    fin.close(); 
    cout << "Данные загружены из файла \"" << file << "\"\n";
}
