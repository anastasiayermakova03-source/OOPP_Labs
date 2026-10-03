#include "Watch.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <windows.h>
using namespace std;

PowerSource::PowerSource(const PowerSource& other) {
	type = other.type;
	capacity = other.capacity;
};

void PowerSource::setType(string t) { type = t; };
void PowerSource::setCapacity(int c) { capacity = c; };

void PowerSource::print() const {
	cout << "Источник заряда : тип [" << type << "]   емкость [" << capacity << "]" << endl;
}

void PowerSource::saveFile(ofstream& f) const {
	f << type << " " << capacity << endl;
};



Dial::Dial(const Dial& other) {
	material = other.material;
	color = other.color;
	diameter = other.diameter;
};

void  Dial::setMaterial(string m) { material = m; };
void Dial::setColor(string c) { color = c; };
void Dial::setDiameter(int d) { diameter = d; };

void Dial::print() const {
	cout << "Циферблат : материал [" << material << "]   цвет [" << color << "]   диаметр [" << diameter << "]" << endl;
};

void Dial::saveFile(ofstream& f) const {
	f << material << " " << color << " " << diameter << endl;
};


Watch::Watch(const Watch& other) {
	brand = other.brand;
	price = other.price;
	if (other.dial != nullptr)
		dial = new Dial(*other.dial);
	else
		dial = nullptr;
	if (other.powersource != nullptr)
		powersource = new PowerSource(*other.powersource);
	else powersource = nullptr;
	indDial = other.indDial;
	indPower = other.indPower;
};

void Watch::setBrand(string br) { brand = br; };
void Watch::setPrice(double pr) { price = pr; };
void Watch::setIndDial(int indD) { indDial = indD; };
void Watch::setIndPower(int indP) { indPower = indP; };
void Watch::setDial(Dial* d) {
	delete dial;
	dial = new Dial(*d); 
}
void Watch::setPower(PowerSource* ps) {
	delete powersource;
	powersource = new PowerSource(*ps);
}

double Watch::calculatePrice() { return price; };
void Watch::saveFile(ofstream& f) const {
	f << brand << " " << price ;
	if (dial) f << " " << indDial;
	if (powersource) f << " " << indPower;
}


MechWatch::MechWatch(const MechWatch& other): Watch(other){
	jewelsCount = other.jewelsCount;
	automaticWinding = other.automaticWinding;
}

void MechWatch::setJewels(int count) { jewelsCount = count; };
void MechWatch::setAutoW(int aw) { automaticWinding = aw; };

double MechWatch::calculatePrice(){
	return (price + jewelsCount * 100);
};

void MechWatch::print() {
	cout << "\nMechanical Watch: " << "Бренд [" << brand << "]  Цена [" << this->calculatePrice() <<
		"]   Кол-во камней [" << jewelsCount << "]   Автоматический завод [" ;
	if (automaticWinding) cout << "есть]";
	else  cout << "нет]";
		cout << endl;
		if (dial)
			dial->print();
		if (powersource) 
			powersource->print();
};

void MechWatch::saveFile(ofstream& f) const {
	Watch::saveFile(f);
	f << " " << jewelsCount << " " << automaticWinding << endl;
};


ElWatch::ElWatch(const ElWatch& other) : Watch(other), backlight(other.backlight), smartFunction(other.smartFunction) {};

void ElWatch::setBacklight(int b) { backlight = b;  }
void ElWatch::setSmart(int smart) { smartFunction = smart; }

void ElWatch::print() {
	cout << "\nElectronic Watch: " << "Бренд [" << brand << "]  Цена [" << this->calculatePrice() <<
		"]   Подсветка ["; 
	if (backlight) cout << "есть]";
	else  cout << "нет]";
	cout << "   Smart-функции [";
	if (smartFunction) cout << "есть]";
	else  cout << "нет]";
	cout << endl;
	if (dial)
		dial->print();
	if (powersource)
		powersource->print();
};

double ElWatch::calculatePrice() {
	double result = price;
	if (smartFunction == 1) result += 600;
	if (backlight== 1) result += 300;
	return result;
};

void ElWatch::saveFile(ofstream& f) const {
	Watch::saveFile(f);
	f << " " << backlight << " " << smartFunction << endl;
};