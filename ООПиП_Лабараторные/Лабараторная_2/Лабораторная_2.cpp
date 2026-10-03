/* 6. Создать класс Array, в котором реализовать методы для работы с
одномерными массивами(как целых чисел, дробных, так и символов) :
    получить пересечение элементов массивов, получить объединение элементов
    массивов.Память под массивы необходимо выделять динамически.Размер
    массивов указывает пользователь.Необходимо обязательно освобождать
    память, выделенную под массивы.Написать свой манипулятор, выводящий
    содержимое массива в виде строки и в виде столбца.*/


#include "Array.h"
#include <iostream>
#include <windows.h>
#include <iomanip>
using namespace std;

ostream& print_row(ostream& stream) {
    stream << "\tМассив-строка:" << endl;
    return stream;
}
/*ostream& print_col(ostream& stream) {
    stream << "\tМассив-столбец:" << endl;
    return stream;
}*/

int menu_array() {
    int comanda;
    cout << "\n===== МЕНЮ МАССИВОВ =====\n";
    while (1) {
        cout << "Выберите тип данных для массива" <<
            "\n| 1. int\n| 2. double\n| 3. char" << endl;
            cout << "| 4. Выход" << endl;
        cout << "\tВаш выбор: ";
        cin >> comanda;
        if (comanda > 0 && comanda < 5)
            return comanda;
        cout << "! Некоррректный ввод !" << endl;
    }
}

int menu_operation() {
    int comanda;
    cout << "\n===== МЕНЮ С ОПЕРАЦИЯМИ =====" << endl;
    while (1) {
        cout << "1. Объединение массивов\n2. Пересечение массивов\n" <<
            "3. Вывести массив в строку\n4. Вывести массив в столбик\n" <<
            "5. Ввести новые массивы\n6. Выход" << endl;
        cout << "\tВаш выбор: ";
        cin >> comanda;
        if (comanda > 0 && comanda < 7)
            return comanda;
        cout << "! Некоррректный ввод !" << endl;
    }
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    while (true) {
        int choice_array = menu_array();
        int size1, size2;
        if (choice_array == 4) {
            cout << "Выход из программы..." << endl;
            return 0;
        }

        cout << "\n---- Массив 1 ----" << endl;
        cout << "Введите количество элементов (1-15): " << endl;
        cin >> size1;
        Array arrayFirst(size1, choice_array);
        arrayFirst.Fill();

        cout << "\n---- Массив 2 ----" << endl;
        cout << "Введите количество элементов (1-15): " << endl;
        cin >> size2;
        Array arraySecond(size2, choice_array);
        arraySecond.Fill();

        int comanda;
        bool newArray = true;
        while (newArray) {
            comanda = menu_operation();
            switch (comanda) {
            case 1: arrayFirst.UnionMassiv(arraySecond);
                break;
            case 2: arrayFirst.IntersectionMassiv(arraySecond);
                break;
            case 3:
                cout << "\t= МАССИВ 1 =\n";
                cout << print_row;
                arrayFirst.PrintRow(cout);
                cout << "\t= МАССИВ 2 =\n";
                cout << print_row;
                arraySecond.PrintRow(cout);
                break;
            case 4:
                cout << "\t= МАССИВ 1 =\n";
                
                //cout << print_col;
                arrayFirst.PrintCol(cout);
                cout << "\t= МАССИВ 2 =\n";
                //cout << print_col;
                arraySecond.PrintCol(cout);
                break;
            case 5:  newArray = false; 
                break;
            case 6: cout << " Выход из программы...\n";
                return 0;
            }
        }
    }
}
