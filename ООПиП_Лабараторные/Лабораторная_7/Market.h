#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

class Customer
{ private:
	string name;
	string card;
public:
	Customer(): name("Без имени"), card("") {};
	Customer(string n, string c): name(n), card(c) {};
	Customer(const Customer& other) {
		name = other.name;
		card = other.card;
	}
	~Customer() {};

	void setName(string n) { name = n; }
	void setCard(string c) { card = c; }
	string getName() const { return name; }
	string getCard() const { return card; }
};

class ProductList
{
protected:
	string product;
	double price;
public:
	ProductList() : product("Пустой продукт"), price(0) {};
	ProductList(string p, double pr) : product(p), price(pr) {};
	ProductList(const ProductList& other) {
		product = other.product;
		price = other.price;
	}
	~ProductList() {};

	void setProduct(string p) { product = p; }
	void setPrice(double pr) { price = pr; }
	string getProduct() const { return product; }
	double getPrice() const { return price; }
	virtual void setTime(int t) {};
	virtual void Show() const;
};


class ServiceList: public ProductList
{
protected:
	int time;
public:
	ServiceList() : ProductList(), time(0) {};
	ServiceList(string p, double pr, int t) : ProductList(p, pr), time(t) {};
	ServiceList(const ServiceList& other) : ProductList(other), time(other.time) {}
	~ServiceList() {};

	void setTime(int t) override { time = t; }
	int getTime() const { return time; }
	void Show() const override;
};


template <class T>
class Array
{
private:
	T* array;
	int size;
public:
	Array(): array(nullptr), size(0) {}
	Array(const Array& other): size(other.size) {
		if (size > 0) {
			size = other.size;
			array = new T[size];
			for (int i = 0; i < size; ++i)
				array[i] = other.array[i];
		}
		else array = nullptr;
	}
	~Array() { delete[] array; array = nullptr; }

	void Add(const T& el);
	T& getEl(int ind) const { return array[ind]; }
	int getSize() const { return size; }
	T* getArray() const { return array; }
	void loadFile(int p, ifstream& file);
};


class Order
{
private:
	Customer user;
	Array<ProductList> products;
	Array<ServiceList> services;
public :
	Order(Customer us) : user(us) {};
	~Order() {}

	Customer getUser() const { return user; }
	void ShowOrder() const;
	void AddProduct(const ProductList& prod) { products.Add(prod); }
	void AddService(const ServiceList& serv) { services.Add(serv); }
	void CalculatePrice() const;
};


void ProductList::Show() const {
	cout << setw(20) << product << "|" << setw(5) << price << "|" << endl;;
}

void ServiceList::Show() const {
	cout << setw(20) << product << "|" << setw(5) << price << "|" << setw(5) << time << "|" << endl;
}

template <class T>
void Array<T>::Add(const T& el) {
	T* temp = new T[size + 1];
	if (size > 0) {
		for (int i = 0; i < size; i++)
			temp[i] = array[i];
	}
	temp[size] = el;
	delete[] array;
	array = temp;
	size++;
	//cout << " Элемент успешно добавлен";
}

template <class T>
void Array<T>::loadFile(int p, ifstream& file) {
	if (!file.is_open()) {
		cout << "Ошибка открытия файла" << endl;
		return;
	}
	T temp;
	string name;
	double price;
	int time;
	if (p == 1) {
		while (file >> name >> price) {
			temp.setProduct(name);
			temp.setPrice(price);
			this->Add(temp);
		}
	}
	if (p == 2) {
		while (file >> name >> price >> time) {
			temp.setProduct(name);
			temp.setPrice(price);
			temp.setTime(time);
			this->Add(temp);
		}
	}
}


void Order::ShowOrder() const {
	cout << "\n ======= ВАША КОРЗИНА ======= \n";
	cout << "" << setw(20) << getUser().getName() << endl;
	cout <<   " ============================ " << endl;

    if (products.getSize() > 0) {
	    cout << "\tТОВАРЫ:\n";
		for (int i = 0; i < products.getSize(); i++)
			products.getEl(i).Show();
	}
	if (services.getSize() > 0) {
		cout << "\n\tУСЛУГИ:\n";
		for (int i = 0; i < services.getSize(); i++)
			  services.getEl(i).Show();
	}
	if (products.getSize() == 0 && services.getSize() == 0) cout << "\tКорзина пуста\n";
}


void Order::CalculatePrice() const {
	cout << "\n ===== СТОИМОСТЬ ВАШЕЙ КОРЗИНЫ ===== \n";

	
	double result1 = 0;
	for (int i = 0; i < products.getSize(); i++) {
		result1 += products.getEl(i).getPrice();
	}
	if (result1 != 0) {
		cout << " Стоимоть товаров: ";
		cout << result1 << "руб." << endl;
	}

	
	double result2 = 0;
	for (int i = 0; i < services.getSize(); i++) {
		result2 += services.getEl(i).getPrice();
	}
	if (result2 != 0) {
		cout << " Стоимоть услуг: ";
		cout << result2 << "руб." << endl;
	}

	double result = result1 + result2;
	cout << " ИТОГ: ";
	if (result == 0) cout << "На данный момент ваша корзина пуста\n";
	else cout << result << "руб." << endl;
};

