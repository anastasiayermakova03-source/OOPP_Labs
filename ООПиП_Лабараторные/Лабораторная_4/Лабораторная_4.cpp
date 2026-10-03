/*6. Создать классы «Транспортное средство», «Самолет», «Поезд», «Автомобиль» с необходимым набором
полей и методов.Создать массив объектов базового класса и заполнить этот массив объектами базового и
производных классов.Определить время и стоимость перевозки для указанных городов и расстояний.
Вывести данные о наиболее быстрой и экономичной поездке на экран и в файл.
Классы должны содержать методы получения и изменения значений всех полей.Все поля классов должны 
быть объявлены с атрибутами private или protected. */

#include "Classes.h"
#include <limits>
#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;
const int Size = 6;

const int Moscow = 715;
const int Pekin = 6500;
const int Tokio = 11500;
const int London = 2170;
const int NewYork = 7200;
const int Peter = 800;
const int Warsaw = 550;
const int Brest = 350;

int ShowCity() { 
    while (true) {
        cout << "\n\t === СПИСОК ГОРОДОВ ===\n";
        cout << " 1)Москва   2)Пекин   3)Токио   4)Лондон   5)Нью-Йорк   6)Санкт-Петербург   " <<
            "7)Варшава   8)Брест" << endl;
        cout << "| Номер города:";
        int city_p;
        cin >> city_p;
        if (city_p < 1 || city_p > 8)
            cout << " ! Некорректный ввод !\n";
        else return city_p;
    }
}

int CityToDistance(int c) {
    switch (c) {
    case 1: c = Moscow; break;
    case 2: c = Pekin; break;
    case 3: c = Tokio; break;
    case 4: c = London; break;
    case 5: c = NewYork; break;
    case 6: c = Peter; break;
    case 7: c = Warsaw; break;
    case 8: c = Brest; break;
    }
    return c;
}

int menu() {
    int choice;
    while (true) {
        cout << "\n\n===== MENU =====\n";
        cout << "| 1. Поиск\n" << "| 0. Выйти\n";
            cout << "\tКоманда: ";
            cin >> choice;
            if (cin.fail()) {
                cin.clear(); // Сбрасываем флаг ошибки
                cin.ignore(10000, '\n');
                cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
            }
            else if (choice != 1 && choice != 0)
                cout << "Такого варианта нет :(" << endl;
            else break;
    }
    return choice;
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

   ifstream in("database.txt");
    if (!in.is_open()) {
        cout << "Ошибка! Невозможно открыть файл" << endl;
        return 0;
    }
    else {
        cout << "Файл успешно открыт, начинаю чтение..." << endl;
    }
    int size = Size;
    Vehicle** Transport = new Vehicle* [size];

    int type;
    int tarif;
    string name ,city;
    int toTimeHours, toTimeMin;
    double speed;

    for (int i = 0; i < size; ++i) {
        if (!(in >> type >> name >> speed >> tarif >> city)) {
            cout << "!!! Ошибка чтения данных на строке " << i + 1 << " !!!" << endl;
            break;
        }
        if (type == 1) {
            double registration;
            in >> registration;
            Transport[i] = new Plane(name, speed, tarif, city, registration);
        }
        else if (type == 2) {
            int comfort;
            in >> comfort;
            Transport[i] = new Train(name, speed, tarif, city, comfort);
        }
        else if (type == 3) {
            double cost;
            in >> cost;
            Transport[i] = new Car(name, speed, tarif, city, cost);
        }
    }
    in.close();
    Vehicle::Show(Transport, size);

    while (true) {
        int choice = menu();
        if (choice == 1) {
            int parametr;
            int choice;
            while (true) {
                cout << "\n ----- ПОИСК ----- ";
                cout << "\n| Вы хотите произвести поиск по параметру: \n" <<
                    " 1) город\t 2) расстояние\n" <<
                    "| ПАРАМЕТР - ";
                cin >> choice;
                if (choice == 1) {
                    int code = ShowCity();
                    parametr = CityToDistance(code);
                    break;
                }
                else if (choice == 2) {
                    cout << "| Введите расстояние = ";
                    cin >> parametr;
                    break;
                }
                else cout << "\n ! Некорректный ввод !\n";
            }

            double min_cost = Transport[0]->calculateCost(parametr);
            double min_time = Transport[0]->calculateTime(parametr);
            int min_time_n = 0, min_cost_n = 0;
            for (int i = 0; i < size; ++i) {

                double current = Transport[i]->calculateTime(parametr);
                if (current < min_time) {
                    min_time = current;
                    min_time_n = i;
                }

                current = Transport[i]->calculateCost(parametr);
                if (current < min_cost) {
                    min_cost = current;
                    min_cost_n = i;
                }
            }

            cout << "\n ===== РЕЗУЛЬТАТЫ ===== ";
            cout << "\n Самый быстрый транспорт " << min_time_n + 1 << ": " <<
                setw(10) << Transport[min_time_n]->GetName() << endl <<
                " | Время: " << setw(10) << min_time << " | " <<
                " | Стоимость: " << setw(10) << Transport[min_time_n]->calculateCost(parametr) << " |\n";

            cout << "\n Самый экономичный транспорт " << min_cost_n + 1 << ": " <<
                setw(10) << Transport[min_cost_n]->GetName() << endl <<
                " | Время: " << setw(10) << Transport[min_cost_n]->calculateTime(parametr) << " | " <<
                " | Стоимость: " << setw(10) << min_cost << " |\n";
        }
        if (choice == 0) {
            cout << "\nВыход из программы...";
         for (int i = 0; i < size; ++i)
             delete Transport[i];
         delete[] Transport;
         return 0;
        }
    }
}
