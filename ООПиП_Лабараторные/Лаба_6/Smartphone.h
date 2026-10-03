#pragma once
#include <fstream>
#include <string>

class Phone {
protected:
	int battery;
	int memory;
	double diagonal;
public:
	Phone();
	Phone(int bat, int mem, double diag);
	Phone(const Phone& other);
	~Phone() {};
	void setBattery(int bat);
	void setMemory(int mem);
	void setDiagonal(double diag);
	int getBattery() const;
	int getMemory() const;
	double getDiagonal() const;
	void saveFilePhone(std::ofstream& s) const;
	void printPhone() const;
};

class Camera {
protected:
	int resolution;
	std::string stabilization;
public:
	Camera();
	Camera(int res, std::string stab);
	Camera(const Camera& other);
	~Camera() {};
	void setResolution(int res);
	void setStabilization(std::string stab);
	int getResolution() const;
	std::string getStabilization() const;
	void saveFileCamera(std::ofstream& s) const;
	void printCamera() const;
};

class Computer {
protected:
	int core;
	double clockFrequency;
public:
	Computer();
	Computer(int core, double clock);
	Computer(const Computer& other);
	~Computer() {};
	void setCore(int core);
	void setClock(double clock);
	int getCore() const;
	double getClock() const;
	void saveFileComputer(std::ofstream& s) const;
	void printComputer() const;
};

class Smartphone: public Phone, public Camera, public Computer {
protected:
	std::string OS;
	std::string model;
public:
	Smartphone() : Phone(), Camera(), Computer(), OS("Android-14"), model("Pixel-9") {}
	Smartphone(std::string OS, std::string model, int bat, int mem, double diag, int res, std::string stab, int core, double clock):
		Phone(bat, mem, diag), Camera(res, stab), Computer(core, clock), OS(OS), model(model) {}
	Smartphone(const Smartphone& other) :
		Phone(other), Camera(other), Computer(other), OS(other.OS), model(other.model) {}
	virtual ~Smartphone() {};
	std::string getOS() const;
	std::string getModel() const;
	void setOS(std::string OS);
	void setModel(std::string model);
	virtual void print() const;
	virtual void saveFile(std::ofstream& s) const;
};


class Gamephone : public Smartphone {
protected:
	bool cooling;
	std::string screen;
public:
	Gamephone() : Smartphone(), cooling(true), screen("OLED") {};
	Gamephone(bool col, std::string screen, std::string OS, std::string model, int bat, int mem, double diag, int res, std::string stab, int core, double clock) :
		Smartphone(OS, model, bat, mem, diag, res, stab, core, clock), cooling(col), screen(screen) { }
	Gamephone(const Gamephone& other) : Smartphone(other), cooling(other.cooling), screen(other.screen) {}
	~Gamephone() {};
	void setCooling(bool col);
	bool getCooling() const;
	void setScreen(std::string screen);
	std::string getScreen() const;
	void print() const override;
	void saveFile(std::ofstream& s) const override;
};

