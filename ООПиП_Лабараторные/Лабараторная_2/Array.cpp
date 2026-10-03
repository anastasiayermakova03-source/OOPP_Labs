#include "Array.h"
#include <iostream>
#include <iomanip>

using namespace std;

Array::Array(int s, int t) : size(s), type(t) {
	cout << "Создаём массив [" << size << "], тип - ";
	if (t == 1) {
		cout << "int" << endl;
		intArray = new int[size];
	}
	if (t == 2) {
		cout << "double" << endl;
		doubleArray = new double[size];
	}
	if (t == 3) 
	{
		cout << "char" << endl;
		charArray = new char[size];
	}
}

Array::Array() : size(5), type(1) {
	cout << "По умолчанию создан массив [5], тип - int" << endl;
	intArray = new int[5];
}

Array::Array(const Array& other) : size(other.size), type(other.type) {
	if (type == 1) intArray = new int[size];
	else if (type == 2) doubleArray = new double[size];
	else if (type == 3) charArray = new char[size];
	CopyData(other);
	cout << "Создана копия" << endl;
}

Array::~Array() {
	if (type == 1) { delete[] intArray; intArray = nullptr; }
	if (type == 2) { delete[] doubleArray; doubleArray = nullptr; }
	if (type == 3) { delete[] charArray; charArray = nullptr; }
		cout << "Память освобождена" << endl;
}

void Array::CopyData(const Array& other) {
	if (other.type == this->type) {
		if (type == 1) {
			int* MyData = intArray;           //Создаем временные указатели опр. типа, указывающее на уже выделенную память. Они потом удаляются, а данные остаются 
			int* OtherData = other.intArray;
			for (size_t i = 0; i < size; i++)
				MyData[i] = OtherData[i];
		}
		if (type == 2) {
			double* MyData = doubleArray;
			double* OtherData = other.doubleArray;
			for (size_t i = 0; i < size; i++)
				MyData[i] = OtherData[i];
		}
		if (type == 3) {
			char* MyData = charArray;
			char* OtherData = other.charArray;
			for (size_t i = 0; i < size; i++)
				MyData[i] = OtherData[i];
		}
	}
	else cout << "Копирование невозможно. Типы данных у массивов не совпадают" << endl;
}

void Array::Fill() {  
	cout << "Введите " << size << " элементов: ";
	if (type == 1)
		for (int i = 0; i < size; i++)
			cin >> intArray[i];
	if (type == 2)
		for (int i = 0; i < size; i++)
			cin >> doubleArray[i];
	if (type == 3)
		for (int i = 0; i < size; i++)
			cin >> charArray[i];
}

void Array::UnionMassiv(const Array& other) { //Объединение
	if (other.type != this->type) {
		cout << "Типы массивов не совпадают" << endl;
		return ;
	}

	if (type == 1) {
		int* newArr = new int[this->size + other.size];
		for (int i = 0; i < size; i++)
			newArr[i] = intArray[i];
		int count = size;

			for (int j = 0; j < other.size; j++) {  //Проверка совпадения элементов во втором массиве
				bool flag = true;
					for (int k = 0; k < count; k++) 
						if (newArr[k] == other.intArray[j])
							flag = false;
						if (flag) // Нет такого элемента
							newArr[count++] = other.intArray[j];
			}
		cout << " === Объединение массивов === " << endl;
		for (int i = 0; i < count; i++)
			cout << " | " << newArr[i];
		cout << " | " << endl;
		delete[] newArr;
	}

	if (type == 2) {
		double* newArr = new double[this->size + other.size];
		for (int i = 0; i < size; i++)
			newArr[i] = doubleArray[i];
		int count = size;

		for (int j = 0; j < other.size; j++) {  //Проверка совпадения элементов во втором массиве
			bool flag = true;
			for (int k = 0; k < count; k++)
				if (newArr[k] == other.doubleArray[j])
					flag = false;
			if (flag) // Нет такого элемента
				newArr[count++] = other.doubleArray[j];
		}
		cout << " === Объединение массивов === " << endl;
		for (int i = 0; i < count; i++)
			cout << " | " << newArr[i];
		cout << " | " << endl;
		delete[] newArr;
	}

	if (type == 3) {
		char* newArr = new char[this->size + other.size];
		for (int i = 0; i < size; i++)
			newArr[i] = charArray[i];
		int count = size;

		for (int j = 0; j < other.size; j++) {  //Проверка совпадения элементов во втором массиве
			bool flag = true;
			for (int k = 0; k < count; k++)
				if (newArr[k] == other.charArray[j])
					flag = false;
			if (flag) // Нет такого элемента
				newArr[count++] = other.charArray[j];
		}
		cout << " === Объединение массивов === " << endl;
		for (int i = 0; i < count; i++)
			cout << " | " << newArr[i];
		cout << " | " << endl;
		delete[] newArr;
	}
}

void Array::IntersectionMassiv(const Array& other) { //Пересечение
	if (other.type != this->type) {
		cout << "Типы массивов не совпадают" << endl;
		return ;
	}

	int MaxSize = (this->size > other.size) ? this->size : other.size;

	if (type == 1) {
		int* newArr = new int[MaxSize];
		int count = 0;

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < other.size; j++) { 
				bool flag = true;
				if (intArray[i] == other.intArray[j]) {
					for (int k = 0; k < count; k++) 
						if (newArr[k] == other.intArray[j])
							flag = false;

						if (flag) // Нет совпадений
							newArr[count++] = other.intArray[j];
				}
				else continue;
			}
		}
		cout << " === Пересечение массивов === " << endl;
		if (count == 0) {
			cout << " Таких элементов нет" << endl;
			delete[] newArr;
			return;
		}
		for (int i = 0; i < count; i++)
			cout << " | " << newArr[i];
		cout << " | " << endl;
		delete[] newArr;
	}

	if (type == 2) {
		double* newArr = new double[MaxSize];
		int count = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < other.size; j++) {
				bool flag = true;
				if (doubleArray[i] == other.doubleArray[j]) {
					for (int k = 0; k < count; k++)
						if (newArr[k] == other.doubleArray[j])
							flag = false;

					if (flag) // Нет совпадений
						newArr[count++] = other.doubleArray[j];
				}
				else continue;
			}
		}
		cout << " === Пересечение массивов === " << endl;
		if (count == 0) {
			cout << " Таких элементов нет" << endl;
			delete[] newArr;
			return;
		}
		for (int i = 0; i < count; i++)
			cout << " | " << newArr[i];
		cout << " | " << endl;
		delete[] newArr;
	}

	if (type == 3) {
		char* newArr = new char[MaxSize];
		int count = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < other.size; j++) {
				bool flag = true;
				if (charArray[i] == other.charArray[j]) {
					for (int k = 0; k < count; k++)
						if (newArr[k] == other.charArray[j])
							flag = false;

					if (flag) // Нет совпадений
						newArr[count++] = other.charArray[j];
				}
				else continue;
			}
		}
		cout << " === Пересечение массивов === " << endl;
		if (count == 0) {
			cout << " Таких элементов нет" << endl;
			delete[] newArr;
			return;
		}
		for (int i = 0; i < count; i++)
			cout << " | " << newArr[i];
		cout << " | " << endl;
		delete[] newArr;
	}
}

ostream& Array::PrintRow(ostream& stream) const {
	switch (type) {
	case 1: {
		int* Row = intArray;
		for (int i = 0; i < size; i++)
			stream << " | " << Row[i];
		stream << " | " << endl;
		break;
	}
	case 2: {
		double* Row = doubleArray;
		for (int i = 0; i < size; i++)
			stream << " | " << Row[i];
		stream << " | " << endl;
		break;
	}
	case 3: {
		char* Row = charArray;
		for (int i = 0; i < size; i++)
			stream << " | " << Row[i];
		stream << " | " << endl;
		break;
	}
	}
	return stream;
}

ostream& Array::PrintCol(ostream& stream) const {
	switch (type) {
	case 1: {
		int* Row = intArray;
		for (int i = 0; i < size; i++)
			stream << " | " << Row[i] << " |\n";
		break;
	}
	case 2: {
		double* Row = doubleArray;
		for (int i = 0; i < size; i++)
			stream << " | " << Row[i] << " |\n";
		break;
	}
	case 3: {
		char* Row = charArray;
		for (int i = 0; i < size; i++)
			stream << " | " << Row[i] << " |\n";
		break;
	}
	}
	return stream;
}
