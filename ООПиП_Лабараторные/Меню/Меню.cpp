#include <cstdlib>
#include <windows.h>
#include <iostream>

using namespace std;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    int choice;
	while (1) {
		cout << " ----- ГЛАВНОЕ МЕНЮ ----- " << endl;
	    cout << "1. Лабораторная 1" << "\n" <<
			"2. Лабораторная 2" << "\n" <<
			"3. Лабораторная 3" << "\n" <<
			"0. Выход" << "\n" << endl;
		cout << "Команда: ";
		cin >> choice;

		switch (choice) {
		case 1: system("start Лабораторная_1.exe");
			break;
		case 2: system("start Лабораторная_2.exe");
			break;
		case 3: cout << "Ничего нет\n";
			break;
		case 0: cout << "Выход из программы..." << endl;
			return 0;
		default: cout << "\nНекорректное значение\n";
			break;
		}
	}
}
