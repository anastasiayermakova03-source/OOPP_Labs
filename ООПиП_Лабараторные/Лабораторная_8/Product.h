#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include <fstream> 
using namespace std;

class Product
{
private: 
	string name;
	int count;
	double price;
public:
	Product(): name("Без имени"), count(0), price(0){}
	Product(string n, int c, double pr): name(n), count(c), price(pr) {}
	Product(const Product& other): name(other.name), count(other.count), price(other.price) {}
	~Product() {}

	void setNamePr(string n) { name = n; }
	void setCount(int c) {
		if (c < 1) throw "Некорректное значение количество товара на складе";
		count = c; }
	void setPrice(double pr) { 
		if (pr <= 0) throw "Некорректная цена";
		price = pr; }
	string getName() const { return name; }
	int getCount() const { return count; }
	double getPrice() const { return price; }
	void printProduct() const;
};


class Purchase
{
private:
	shared_ptr<Product> product;
	int size=0;
	double totalPrice;
public:
	Purchase(): product(nullptr), size(0), totalPrice(0) {}
	Purchase(shared_ptr<Product> p, int s) {
		product = p;
		size = s;
		totalPrice = product->getPrice() * size;
	}
	Purchase(const Purchase& other) {
		product = other.product;
		size = other.size;
		totalPrice = other.totalPrice;
	}
	~Purchase() {}

	int getSize() const{ return size; }
	double getTotalPrice() const { return totalPrice; }
	void printPurchase() const {
		if (product)
		cout << " Товар [" << product->getName() << "]:\tКол-во товара [" << size << "] " << endl;
	}
};


class Customer
{
private:
	string name;
	Purchase total[30];
	int currentCount = 0;
public:
	Customer(): name("Неизвестный"), currentCount(0) {}
	Customer(string n): name(n) {}
	Customer(const Customer& other) {
		currentCount = other.currentCount;
		for (int i = 0; i < currentCount; i++)
			total[i] = other.total[i];
	}
    ~Customer() {}

	void setName(string n) { name = n; }
	string getName() const { return name; }
	int getCurrentCount() const { return currentCount; }
	void addPurchase(const Purchase& purchase) {
		if(currentCount < 30)
		total[currentCount++] = purchase;
	}
	void printCheck() {
		cout << "\n ---- ВАШ ЧЕК : " << name << " --------- \n";
		double sumCheck = 0;
		if (currentCount == 0) {
			cout << " Ваш список покупок пуст" << endl;
			return;
		}
		for (int i = 0; i < currentCount; i++) {
			cout << i + 1 << ") ";
			total[i].printPurchase();
			sumCheck += total[i].getTotalPrice();
			cout << "\tОбщая стоимость [" << total[i].getTotalPrice() << "]\n\n";
		}
		cout << " -------------------------------\n";
		cout << " ИТОГО: " << sumCheck << " руб." << endl;
	}
	void showPurchase() {
		cout << "\n ------ ВАША КОРЗИНА : " << name << "-------\n";   
		if (getCurrentCount() == 0) cout << "  Корзина пуста\n";
		else
		for (int i = 0; i < currentCount; i++) {
			cout << " [" << i + 1 << "] "; 
			total[i].printPurchase();
		}
	}
};


template <class T>
class Inventary
{
private: 
	T array[100];
	int kol = 0;
public:
	Inventary(): kol(0) {}
	Inventary(T& ex) {
		array[kol++] = ex;
	}
	Inventary(const Inventary<T>& other) {
		kol = other.kol;
		for (int i = 0; i < kol; i++)
			array[i] = other.array[i];
	}
	~Inventary() {}

	T& getProduct(int index) {
		return array[index];
	}
	int getKol() const { return kol; }
	void setKol(int k) { kol = k; }
	double CalculateTotal();
	void printInv(int ch) const;
	void addProduct(shared_ptr<Product> product) {
		if (kol < 100)
			array[kol++] = product;
		else cout << "На складе нет места\n";
	}
	void loadFile(ifstream& file);
};


class Transaction
{
private:
	Product temp;
	bool ex;
public:
	void begin(const Product& other) {
		temp = other;
		ex = true;
	}
	void commit() { ex = false; }
	void rollback(Product& other) { other = temp; ex = false; }
};

void Product::printProduct() const {
	cout << setw(20) << name << "|" << setw(15) << count << "|" << setw(5) << price << "|" << endl;
}


template<class T>
double Inventary<T>::CalculateTotal() {
	double totalPr = 0;
	for (int i = 0; i < kol; i++)
		totalPr += array[i].getPrice();
	if (totalPr == 0) {
		cout << " Общая стоимость покупок равна нулю, так как покупок еще не происходило" << endl;
		return 0;
	}
	else return totalPr;
}

template <class T>
void Inventary<T>::loadFile(ifstream& file) {
	if (!file.is_open()) {
		cout << "Ошибка открытия файла" << endl;
		return;
	}
	string n;
	int c;
	double p;
	while (file >> n >> c >> p) {
		auto temp = make_shared<Product>(n, c, p);
		this->addProduct(temp);
	}
}

template<class T>
void Inventary<T>::printInv(int ch) const {
		array[ch]->printProduct();
}