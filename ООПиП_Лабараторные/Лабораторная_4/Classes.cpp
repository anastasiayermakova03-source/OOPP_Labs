#include "Classes.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <limits>
using namespace std;


Vehicle::Vehicle() : name("Без названия"), speed(100), tarif(10), city("Москва"),
		toTimeHours(0), toTimeMin(0) {}
Vehicle::Vehicle(string n, double sp, int t, string c)
		: name(n), speed(sp), tarif(t), city(c), toTimeHours(0), toTimeMin(0) {}
Vehicle::~Vehicle() {}
	
int Vehicle::GetType() const { return 0; };

void Vehicle::SetTime() {
		cout << "| Введите время (часы 0-23): ";
		while (!(cin >> this->toTimeHours) || this->toTimeHours < 0 || this->toTimeHours > 23) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Ошибка! Вы ввели буквы. Пожалуйста, введите число: ";
			}
			else if (this->toTimeHours < 0 || this->toTimeHours > 23) {
				cout << "Ошибка! Время должно быть >0 и <24: ";
			}
		}
		cout << "| Введите время (минуты 0-59): ";
		while (!(cin >> this->toTimeMin) || this->toTimeMin < 0 || this->toTimeMin > 59) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Ошибка! Вы ввели буквы. Пожалуйста, введите число: ";
			}
			else if (this->toTimeMin < 0 || this->toTimeMin > 59) {
				cout << "Ошибка! Время должно быть >0 и <60: ";
			}
		}
}

string Vehicle::GetTime() const {
		string Time1, Time2;
		if (this->toTimeHours < 10)
			Time1 = '0';
		Time1 += to_string(this->toTimeHours);

		if (this->toTimeMin < 10)
			Time2 += '0';
		Time2 += to_string(this->toTimeMin);
		return Time1 + ":" + Time2;
}


void Vehicle::SetName() { cout << "\n| Введите новое имя: "; cin >> name;}
string Vehicle::GetName() const { return name; }

void Vehicle::SetCity() {cout << "\n| Введите новый город: "; cin >> city;}
string Vehicle::GetCity() const { return city; }

void Vehicle::SetSpeed() { cout << "\n| Введите новую скорость: "; cin >> speed;}
double  Vehicle::GetSpeed() const {	return speed; }

void Vehicle::SetTarif() { cout << "\n| Введите новый тариф (за час): "; cin >> tarif; }
int Vehicle::GetTarif() const{ return tarif; }

double Vehicle::calculateCost(int r) { return tarif * calculateTime(r);}
double Vehicle::calculateTime(int r) {  return r / speed; }

void Vehicle::Show(Vehicle** obj, int size) {
		cout << "\n\t======= ТАБЛИЦА =======\n";
		cout << " | " << setw(6) << "Номер" ;
		cout << " | " << setw(10) << "Тип" << " | " << setw(10) << "Имя" <<
			" | " << setw(10) << "Город" << 
			" | " << setw(10) << "Скорость" <<
			" | " << setw(15) << "Тариф (за час)" << " | " << endl;
		cout << " " << string(80,'-') << endl;

		for (int i = 0; i < size; ++i) {
			cout << " | " << setw(6) << i + 1;
			if (obj[i] == nullptr) continue;

			switch (obj[i]->GetType()) {
			case 1:
				cout << " | " << setw(10) << "Самолет";
				break;
			case 2:
				cout << " | " << setw(10) << "Поезд";
				break;
			case 3:
				cout << " | " << setw(10) << "Автомобиль";
				break;
			default:
				cout << " | " << setw(10) << "Неизвестно";
				break;
			}
			cout << " | " << setw(10) << obj[i]->GetName() <<
			" | " << setw(10) << obj[i]->GetCity() << 
		    " | " << setw(10) << obj[i]->GetSpeed() << 
		    " | " << setw(15) << obj[i]->GetTarif() << " | " << endl;
		}
}


Plane::Plane() : Vehicle() {
		this->registration = 2;
}
Plane::Plane(string n, double sp, int t, string c, double reg)
	: Vehicle(n, sp, t, c), registration(reg) {
}
Plane::~Plane() {}
int Plane::GetType() const { return 1; }  //обозначит тип "самолет"
double Plane::calculateTime(int r) {
	return r/GetSpeed() + this->registration;
};
double Plane::calculateCost(int r) {
	return  GetTarif() * (r / GetSpeed());
}


Train::Train() : Vehicle() {
	this->typeComf = 2;  // 1 - Повышенный Комфорт, 2 - Стандарт, 3 - Базовый
}
Train:: Train(string n, double sp, int t, string c, int comf)
	: Vehicle(n, sp, t, c), typeComf(comf) {
}
Train::~Train() {};
int Train::GetType() const { return 2; }  //обозначит тип "поезд"

double Train::calculateTime(int r) {
	return r / GetSpeed();
};

double Train::calculateCost(int r) {
	double tarif = GetTarif();
	if (this->typeComf == 1)
		tarif = tarif * 5;
	if (this->typeComf == 2)
		tarif = tarif*2;
	return  tarif * (r / GetSpeed());
}


Car::Car() : Vehicle(), costResources(2.5) {}
Car::Car(string n, double sp, int t, string c, double cost)
	: Vehicle(n, sp, t, c), costResources(cost) {
}
Car::~Car() {}
int Car::GetType() const { return 3; }  //обозначит тип "машина"

double Car::calculateTime(int r) {
	return r / GetSpeed();
};
double Car::calculateCost(int r) {
	return  (GetTarif() + costResources) * (r / GetSpeed());
}