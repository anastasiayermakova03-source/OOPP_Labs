#pragma once
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Toy
{
	string name;
	double price;
public:
	 Toy(string n, double pr): name(n), price(pr) {}
	 void setName(string n)  { name = n; }
	 void setPrice(double pr)  { price = pr; }
	 string getName() const{ return name; }
	 double getPrice() const { return price; }
	 virtual void print() const {
		 cout << setw(30) << name <<  "|" << setw(7) << setprecision(5) << price << "|";
	 }
	 virtual ~Toy() {}
};

class Car: public Toy 
{
	string color;
	string model;
public:
	Car(string n, double pr, string c, string m): 
		Toy(n,pr), color(c), model(m) {};
	void setColor(string c) { color = c; }
	void setModel(string m) { model = m; }
	string getColor() const { return color; }
	string getModel() const { return model; }
	void print() const override {
		Toy::print();
		cout << setw(15) << color << "|" << setw(15) << model;
	}
	~Car() override {}
};

class Doll : public Toy
{
	string brand;
	bool sing;
public:
	Doll(string n, double pr, string br, bool s) :
		Toy(n, pr), brand(br), sing(s) {
	};
	void setBrand(string br) { brand = br; }
	void setSing (bool s) {  sing = s; }
	string getBrand() const { return brand; }
	string getSing() const { return "Не поет"; }
	void print() const override {
		Toy::print();
		cout << setw(15) << brand << "|" << setw(15) << getSing();
	}
	~Doll() override {}
};

class Constructor : public Toy
{
	int details;
	string level;
public:
	Constructor(string n, double pr, int d, string l) :
		Toy(n, pr), details(d), level(l) {
	};
	void setDetails(int d) { details = d; }
	void setLevel(string l) { level = l; }
	int getDetails() const { return details; }
	string getLevel() const { return level; }
	void print() const override {
		Toy::print();
		cout << setw(15) << details << "|" << setw(15) << getLevel();
	}
	~Constructor() override {}
};

class OrderOne 
{
	int count;
	const Toy* toys;
public:
	OrderOne() { count = 0; toys = nullptr; }
	OrderOne(const Toy* t, int c): count(c), toys(t) { 
		if (c < 0) {
			cout << "Количество не может быть отрицательным" << endl;
			count = 0;
		}
		if (t == nullptr) 
			cout << "Неправильно выбрана игрушка" << endl;
	}

	void setCount() {
		int kol;
		while (true) {
			cout << "Введите новое количество [" << toys->getName() << "]: ";
			cin >> kol;
			if (cin.good()) break;
			cin.clear();
			cout << "Некорректный ввод" << endl;
			cin.ignore(100, '\n');
		}
		if (kol == 0) {
			cout << "Товар будет удален из корзины\n";
		}
		else
			if (kol < 0) {
				cout << "Ошибка! Введено отрицательное кол-во товаров.\n";
				return; }
		count = kol;
	}

	void setToys(const Toy* t) { 
		if (t == nullptr)
			cout << "Ошибка" << endl;
		else 
			toys = t; 
	}

	string getOrderOne() const { return toys->getName(); }
	int getCount() const { return count; }
	void printOrderOne() const {
		cout << setw(15) << getOrderOne() << " -" << setw(3) << count ;
	}
	double calculateCost() const {
		return toys->getPrice() * count;
	}
};


class Order
{
	static const int max_item = 10;
	OrderOne myOrder[max_item];
	int current;
public:
	Order() { current = 0; }
	int getCurrent() const { return current; }

	int getCount(int num) {
		return myOrder[num-1].getCount();
	}

	void addOrderOne(const OrderOne one) {
		myOrder[current++] = one;
	}

	void deleteOrderOne(int num) {
		if (num > current || num < 1) {
			cout << "Ошибка" << endl;
			return;
		}

		for (int i = num-1 ; i < current-1; i++) {
				myOrder[i] = myOrder[i+1];
		}
		myOrder[current - 1] = OrderOne();
		current--;
	}

	void clearOrderOne() {
		for (int i = 0; i < current; i++) 
		myOrder[i] = OrderOne();
		current = 0;
		cout << "Корзина очищена успешно\n";
	}

	void changeOrderOne(int num) {
		if (num > current || num < 1) {
			cout << "Ошибка" << endl;
			return;
		}
		myOrder[num - 1].setCount();
	}

	void printOrder() const {
		if (current == 0) {
			cout << "Ваша корзина пуста" << endl;
			return;
		}
		cout << "\n ---- КОРЗИНА ----\n";
		cout << string(40, '-') << endl;
		cout << "|" << setw(10) << "товар" << " - " << setw(10) << "кол-во" << "|" << endl;
		cout << string(40, '-') << endl;
		for (int i = 0; i < current; i++) {
			cout << "|";
			myOrder[i].printOrderOne();
			cout << "|" << endl;
		}
		cout << string(40, '-') << endl;
	}

	double totalPrice() const {
		if (current == 0) {
			cout << "Ваша корзина пуста" << endl;
			return -1;
		}
		double res = 0;
		for (int i = 0; i < current; i++)
			res += myOrder[i].calculateCost();
		return res;
	}
};

