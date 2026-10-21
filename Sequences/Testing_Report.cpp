
#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include "TestingManager.h"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try {
		TestingManager pusk;
		pusk.run();
	}
	catch (const exception& ex) {
		cout << "Исключение: " << ex.what() << endl;
		return -1;
	}
}