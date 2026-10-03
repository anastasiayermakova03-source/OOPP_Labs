#include "Smartphone.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>
using namespace std;


Phone::Phone(): battery(36), memory(32), diagonal(6.5) {}
Phone::Phone(int bat, int mem, double diag) : battery(bat), memory(mem), diagonal(diag) {};
Phone::Phone(const Phone& other) {
	battery = other.battery;
	memory = other.memory;
	diagonal = other.diagonal;
};
void Phone::setBattery(int bat) { battery = bat; }
void Phone::setMemory(int mem) { memory = mem; }
void Phone::setDiagonal(double diag) { diagonal = diag; }
int Phone::getBattery() const { return battery; }
int Phone::getMemory() const { return memory; }
double Phone::getDiagonal() const { return diagonal; }
void Phone::saveFilePhone(ofstream& s) const {
	s << battery << " " << memory << " " << diagonal << " ";
}
void Phone::printPhone() const {
	cout << " Батарея[" << battery << "]   " <<
		"ОП[" << memory << "]   " << "Диагональ[" << diagonal << "]" << endl;
}


Camera::Camera(): resolution(48), stabilization("Оптическая") {}
Camera::Camera(int res, string stab): resolution(res), stabilization(stab) {}
Camera::Camera(const Camera& other) {
	resolution = other.resolution;
	stabilization = other.stabilization;
}
void Camera::setResolution(int res) { resolution = res; }
void Camera::setStabilization(string stab) { stabilization = stab; }
int Camera::getResolution() const { return resolution; }
string Camera::getStabilization() const { return stabilization; }
void Camera::saveFileCamera(ofstream& s) const {
	s << resolution << " " << stabilization << " ";
}
void Camera::printCamera() const {
	cout << " Разрешение[" << resolution << "]   " << "Тип стабилизации[" << stabilization << "] " << endl;
}


Computer::Computer() : core(8), clockFrequency(120) {};
Computer::Computer(int core, double clock) : core(core), clockFrequency(clock) {};
Computer::Computer(const Computer& other) {
	core = other.core;
	clockFrequency = other.clockFrequency;
}
void Computer::setCore(int core) { core = core; }
void Computer::setClock(double clock) { clockFrequency = clock; }
int Computer::getCore() const { return core; }
double Computer::getClock() const { return clockFrequency; }
void Computer::saveFileComputer(ofstream& s) const {
	s << core << " " << clockFrequency << " ";
}
void Computer::printComputer() const { 
	cout << " Кол-во ядер[" << core << "]   " << "Такт. частота[" << clockFrequency << "] " << endl;
}



string Smartphone::getOS() const { return OS; };
string Smartphone::getModel() const { return model; }
void Smartphone::setOS(string OS) { OS = OS; }
void Smartphone::setModel(std::string model) { model = model; }
void Smartphone::print() const {
	cout << " Smartphone " << endl << " ОС и ее версия[" << OS << "]   Модель[" << model << "]\n";
	printPhone(); printCamera(); printComputer();
}
void Smartphone::saveFile(ofstream& s) const {
	s << "Smartphone " << OS << " " << model << " ";
	saveFilePhone(s), saveFileCamera(s), saveFileComputer(s);
	s << endl;
}


void Gamephone::setCooling(bool col) { cooling = col; }
bool Gamephone::getCooling() const { return cooling; }
void Gamephone::setScreen(string screen) { screen = screen; }
string Gamephone::getScreen() const { return screen; }
void Gamephone::print() const {
	cout << " Gaming phone " << endl;
	cout << " ОС и ее версия[" << OS << "]   Модель[" << model << "]\n";
	cout << " Охлаждение[";
	 if (cooling) cout << "есть]";
	 else cout << "нет]";
	cout << "   Экран[" << screen << "]\n";
	printPhone(); printCamera(); printComputer();
}
void Gamephone::saveFile(std::ofstream& s) const {
	s << "Gamephone " << OS << " " << model << " " << cooling << " " << screen << " ";
	saveFilePhone(s), saveFileCamera(s), saveFileComputer(s);
	s << endl;
};
