#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Класс Employee
class Employee {
private:
    static int counter; // Статическая переменная для общего подсчета
    int id;             // Порядковый номер конкретного объекта
    string name;        // Поле 1: Имя
    double salary;      // Поле 2: Зарплата

public:
    // 1. Конструктор без параметров
    Employee() {
        id = ++counter;
        name = "Нет данных";
        salary = 0.0;
    }

    // 2. Конструктор с параметрами
    Employee(string empName, double empSalary) {
        id = ++counter;
        setName(empName);
        setSalary(empSalary);
    }

    // 3. Конструктор копирования
    Employee(const Employee& other) {
        id = ++counter; // Новый объект - новый порядковый номер
        name = other.name;
        salary = other.salary;
    }

    // 4. Деструктор
    ~Employee() {
        // Вызывается при удалении объекта
    }

    // 5. Методы установки с проверкой (Сеттеры)
    void setName(string empName) {
        if (!empName.empty()) {
            name = empName;
        }
        else {
            name = "Аноним";
        }
    }

    void setSalary(double empSalary) {
        if (empSalary >= 0) {
            salary = empSalary;
        }
        else {
            salary = 0.0; // Защита от отрицательных значений
        }
    }

    // Геттеры
    int getId() const { return id; }
    string getName() const { return name; }
    double getSalary() const { return salary; }

    // Метод вывода порядкового номера (по заданию)
    void printMyNumber() const {
        cout << "Мой порядковый номер: " << id << endl;
    }

    // Вывод всей информации
    void display() const {
        cout << "[" << id << "] Сотрудник: " << name << " | Зарплата: " << salary << endl;
    }

    // Запись в файл
    void saveToFile(ofstream& file) const {
        if (file.is_open()) {
            file << id << ";" << name << ";" << salary << endl;
        }
    }
};

// Инициализация статического счетчика
int Employee::counter = 0;

// Логика лабораторной работы №7
void runLab7() {
    int n;
    cout << "Введите количество сотрудников: ";
    cin >> n;

    if (n <= 0) return;

    // Создаем динамический массив указателей на объекты
    // Это позволяет инициализировать каждый объект отдельно после ввода данных
    Employee** staff = new Employee * [n];

    for (int i = 0; i < n; i++) {
        string nme;
        double sal;
        cout << "\nВвод данных для сотрудника " << i + 1 << ":" << endl;
        cout << "Имя: ";
        cin.ignore();
        getline(cin, nme);
        cout << "Зарплата: ";
        cin >> sal;

        // Создание объекта через конструктор с параметрами
        staff[i] = new Employee(nme, sal);
    }

    // Вывод на экран и в файл
    ofstream fout("output.txt");
    cout << "\n--- Список сотрудников ---" << endl;

    for (int i = 0; i < n; i++) {
        staff[i]->printMyNumber(); // Метод из условия
        staff[i]->display();       // Общий вывод
        staff[i]->saveToFile(fout); // В файл
    }

    fout.close();
    cout << "\nДанные также сохранены в файл output.txt" << endl;

    // Очистка памяти (ВАЖНО при работе без векторов)
    for (int i = 0; i < n; i++) {
        delete staff[i]; // Удаляем каждый объект
    }
    delete[] staff; // Удаляем массив указателей
}

int main() {
    setlocale(LC_ALL, "Russian");
    int choice;

    do {
        cout << "\n--- Главное меню ---" << endl;
        cout << "7. Лабораторная работа 7" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите номер работы: ";
        cin >> choice;

        if (choice == 7) {
            runLab7();
        }
        else if (choice != 0) {
            cout << "Работа с таким номером не найдена." << endl;
        }

    } while (choice != 0);

    return 0;
}