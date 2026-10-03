#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;

class PowerSource
{
	string type; // тип: батарейка, аккумулятор, пружина
	int capacity;  //ёмкость/запас хода
public:
	PowerSource() : type("аккумулятор"), capacity(100) {};
	PowerSource(string t, int c) : type(t), capacity(c) {};
	PowerSource(const PowerSource& other);
	~PowerSource() {};

	string getType() const { return type; }
	int getCapacity() const { return capacity; }
	void setType(string t);
	void setCapacity(int c);

	void print() const;
	void saveFile(ofstream& f) const;
};

class Dial 
{
	string material;
	string color;
	int diameter;
public:
	Dial() : material("стекло"), color("черный"), diameter(30) {};
	Dial(string m, string c, int d): material(m), color(c), diameter(d) {};
	Dial(const Dial& other);
	~Dial() {};

	string getMaterial() const { return material; }
	string getColor() const { return color; }
	int getDiameter() const { return diameter; }
	void setMaterial(string m);
	void setColor(string c);
	void setDiameter(int d);

	void print() const;
	void saveFile(ofstream& f) const;
};

class Watch
{
protected:
	string brand;
	double price;
	Dial* dial;
	PowerSource* powersource;
	int indDial;
	int indPower;
public:
	Watch() : brand("Без имени"), price(0), dial(nullptr), powersource(nullptr), indDial(0), indPower(0) {}
	Watch(string br, double pr, Dial* d, PowerSource* ps, int indDial, int indPower) :
	brand(br), price(pr), indDial(indDial), indPower(indPower) {
	dial = (d ? new Dial(*d) : nullptr);
	powersource = (ps ? new PowerSource(*ps) : nullptr);
	};
	Watch(const Watch& other);
	virtual ~Watch() {
		delete dial;
		delete powersource;
	};

	string getBrand() const { return brand; }
	double getPrice() const { return price; }
	Dial* getDial() const { return dial; }
	PowerSource* getPower() const { return powersource; }
	int getIndDial() const { return indDial; }
	int getIndPower() const { return indPower; }
	void setBrand(string br);
	void setPrice(double pr);
	void setDial(Dial* d);
	void setPower(PowerSource* ps);
	void setIndDial(int indDial);
	void setIndPower(int indPower);

	virtual void print() = 0;
	virtual double calculatePrice();
	virtual void saveFile(ofstream& f) const;
};


class MechWatch : public Watch
{
private:
	int jewelsCount;
	int automaticWinding;
public:
	MechWatch() : Watch(), jewelsCount(21), automaticWinding(1) {}
	MechWatch(string br, double pr,  Dial* d, PowerSource* ps, int indD, int indP, int count, int aw) :
		Watch(br, pr, d, ps, indD, indP), jewelsCount(count), automaticWinding(aw) {};
	MechWatch(const MechWatch& other);
	~MechWatch() {};

	int getJewels() const { return jewelsCount; }
	bool getAutoW() const { return automaticWinding; }
	void setJewels(int count);
	void setAutoW(int aw);

	void print() override;
	double calculatePrice() override;
	void saveFile(ofstream& f) const override;
};


class ElWatch : public Watch
{
private:
	int backlight;
	int smartFunction;
public:
	ElWatch() : Watch(), backlight(0), smartFunction(0) {}
	ElWatch(string br, double pr, Dial* d, PowerSource* ps, int indD, int indP, int b, int smart) :
		Watch(br, pr, d, ps, indD, indP), backlight(b), smartFunction(smart) {
	};
	ElWatch(const ElWatch& other);
	~ElWatch() {};

	bool getBacklight() const { return backlight; }
	bool getSmart() const { return smartFunction; }
	void setBacklight(int b);
	void setSmart(int smart);

	void print() override;
	double calculatePrice() override;
	void saveFile(ofstream& f) const override;
};