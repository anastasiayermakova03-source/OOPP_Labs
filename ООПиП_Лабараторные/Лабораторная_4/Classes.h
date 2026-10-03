#pragma once
#include <iostream>
#include <string>
using namespace std;

class Vehicle {
private:
	string name;
	int toTimeHours;
	int toTimeMin;
	double speed;
	string city;
	int tarif;
public:
	Vehicle();
	Vehicle(string n, double sp, int t, string c);
	virtual ~Vehicle();

	virtual int GetType() const;
	void SetTime();
	string GetTime() const;
	void SetName();
	string GetName() const;
	void SetCity();
	string GetCity() const;
	void SetSpeed();
	double GetSpeed() const;
	void SetTarif();
	int GetTarif() const;

	virtual double calculateCost(int r);
	virtual double calculateTime(int r);

	static void Show(Vehicle** obj, int size);
};


class Plane : public Vehicle
{
private:
	double registration;
public:
	Plane();
	Plane(string n, double sp, int t, string c, double reg);
	~Plane();
	int GetType() const override;
    double calculateCost (int r) override;
	double calculateTime(int r) override;
};


class Train : public Vehicle
	{
	private:
		int typeComf;
	public:
		Train();
		Train(string n, double sp, int t, string c, int comf);
		~Train();
		int GetType() const override;
		double calculateCost(int r) override;
		double calculateTime(int r) override;
};


class Car : public Vehicle
{
	private:
	    double costResources;
	public:
		Car();
		Car(string n, double sp, int t, string c, double cost);
		~Car();
		int GetType() const override;
		double calculateCost(int r) override;
		double calculateTime(int r) override;
};
