#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

class MyExc: public exception
{
protected:
	string message;
	string obj;
public:
	MyExc(string m, string obj) : message(m), obj(obj) {
		cout << "\nОшибка: " << message << endl;
	};
	const char* what() const noexcept override 
	{ return message.c_str(); }  //чтобы преобразовать строку в массив символов
};

class ExcPrice: public MyExc 
{
private:
	double errorPrice;
public:
	ExcPrice(string m, string obj, double pr): MyExc(m, obj),errorPrice(pr) {
		cout << "Вы ввели неположительное значение для стоимости товара или услуги [" << 
		obj << "] : " << errorPrice << endl;
	};
};

class ExcCount : public MyExc
{
private:
	int errorCount;
public:
	ExcCount(string m, string obj, int count) : MyExc(m, obj), errorCount(count) {
		cout << "Вы ввели неположительное значение для количества доступного товара [" <<
			obj << "] : " << errorCount << endl;
	};
};

class ExcCountAvail : public MyExc 
{
private: 
	int errorCount;
	int count;
public:
	ExcCountAvail(string m, string obj, int erC, int c) : MyExc(m, obj), errorCount(erC), count(c) {
		cout << "Вы не можете купить товар [" << obj << "] в количестве = " << errorCount << endl;
		cout << "Максимальное количество, доступное для заказа: " << count << endl;
	}
};

class ExcMemory : public MyExc
{
public:
	ExcMemory(string m, string obj) : MyExc(m, obj) {
		cout << "Под товар [" << obj << "] не удалось выделить память. Попробуйте позже" << endl;
	}
};

class ExcOrder : public MyExc
{
public:
	ExcOrder(string m, string obj) : MyExc(m, obj) {
		cout << "В корзине пользователя [" << obj << "] нет ни товаров, ни услуг" << endl;
	};
};

class ExcFile : public MyExc
{
private:
	string file;
public:
	ExcFile(string m, string obj, string f) : MyExc(m, obj), file(f) {
		cout << "Произошла ошибка при открытии файла \"" << file << "\"\n";
	}
};


class Customer
{
private:
	string name;
	string card;
public:
	Customer() : name("Без имени"), card("") {};
	Customer(string n, string c) : name(n), card(c) {};
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
	int count;
public:
	ProductList() : product("Пустой продукт"), price(0), count(0) {};
	ProductList(string p, double pr, int c) : product(p), price(pr), count(c) {};
	ProductList(const ProductList& other) {
		product = other.product;
		price = other.price;
		count = other.count;
	}
	virtual ~ProductList() {};

	void setProduct(string p) { product = p; }
	void setPrice(double pr) { 
		if (pr <= 0)
			throw ExcPrice("Некорректный ввод стоимости", product, pr); //исключение о некорректной цене
		else price = pr; 
	}
	void setCount(int c) {
		if (c <= 0) {
			throw ExcCount("Некорректный ввод количества товара", product, c); //исключение о некорректном количестве
		}
		else count = c;
	}
	string getProduct() const { return product; }
	double getPrice() const { return price; }
	int getCount() const { return count; }
	virtual void setForProduct(string f) {};
	void Show() const {
		cout << setw(40) << product << "|" << setw(5) << price << "|" << setw(15) << count << "|" << endl;
	};
	void ShowOrder() const {
		cout << setw(40) << product << "|" << setw(5) << price << "|" << endl;
	};
};


class ServiceList : public ProductList
{
protected:
	string forProduct;
public:
	ServiceList() : ProductList(), forProduct("Без названия") {};
	ServiceList(string p, double pr, int count, string f) : ProductList(p, pr, count), forProduct(f) {};
	ServiceList(const ServiceList& other) : ProductList(other), forProduct(other.forProduct) {}
	~ServiceList() {};

	void setForProduct(string f) override { forProduct = f; }
	string getForProduct() const { return forProduct; }
};


template <class T>
class Array
{
private:
	T* array;
	int size;
public:
	Array() : array(nullptr), size(0) {}
	Array(const Array& other) : size(other.size) {
		if (size > 0) {
			size = other.size;
			array = new(nothrow) T[size];
			if (!array) {
				string name = other.array[0].getProduct();
				throw ExcMemory("Неудачное выделение памяти", name);  // исключение выделения памяти
			}
			else
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
	void loadFile( ifstream& file);
};


class Order
{
private:
	Customer user;
	Array<ProductList> products;
	Array<ServiceList> services;
public:
	Order(Customer us) : user(us) {};
	~Order() {}

	Customer getUser() const { return user; }
	void ShowOrder() const;
	void AddProduct(const ProductList& prod) { products.Add(prod); }
	void AddService(const ServiceList& serv) { services.Add(serv); }
	void CalculatePrice() const;
};


template <class T>
void Array<T>::Add(const T& el) {
	T* temp = new(nothrow) T[size + 1];
	if (!temp) {
		string name = el.getProduct();
		throw ExcMemory("Неудачное выделение памяти", name); // исключение выделения памяти
	}
		if (size > 0) {
			for (int i = 0; i < size; i++)
				temp[i] = array[i];
		}
	temp[size] = el;
	delete[] array;
	array = temp;
	size++;
}

template <class T>
void Array<T>::loadFile(ifstream& file) {
	if (!file.is_open()) {
		cout << "Ошибка открытия файла" << endl;
		return;
	}
	string name;
	double price;
	int count;
	while (file >> name >> price >> count) {
		T temp;
		temp.setProduct(name);
		temp.setPrice(price);
		temp.setCount(count);
		this->Add(temp);
	}
}


void Order::ShowOrder() const {
	cout << "\n\t ======= ВАША КОРЗИНА ["  << getUser().getName() << "] ======== \n";
	if (products.getSize() > 0) {
	cout << "\n\t\t\tТОВАРЫ:\n";
	for (int i = 0; i < products.getSize(); i++)
	    products.getEl(i).ShowOrder();
	}
	if (services.getSize() > 0) {
		cout << "\n\t\t\tУСЛУГИ:\n";
		for (int i = 0; i < services.getSize(); i++)
			services.getEl(i).ShowOrder();
	}
	if (products.getSize() == 0 && services.getSize() == 0) 
		throw ExcOrder("Пустая корзина", getUser().getName()); // исключение о пустоте корзины
	cout << "\t ====================================== " << endl;
}


void Order::CalculatePrice() const {
	cout << "\n ===== СТОИМОСТЬ ВАШЕЙ КОРЗИНЫ ===== \n";

	double result1 = 0;
	for (int i = 0; i < products.getSize(); i++) {
		result1 += products.getEl(i).getPrice();
	}
	if (result1 != 0) {
		cout << " Общая стоимоть товаров: ";
		cout << result1 << "руб." << endl;
	}

	double result2 = 0;
	for (int i = 0; i < services.getSize(); i++) {
		result2 += services.getEl(i).getPrice();
	}
	if (result2 != 0) {
		cout << " Общая стоимоть услуг: ";
		cout << result2 << "руб." << endl;
	}

	double result = result1 + result2;
	if (result == 0) 
		throw ExcOrder("Пустая корзина", getUser().getName()); // исключение, что корзина пуста
	else 
		cout << " ИТОГОВАЯ СУММА: " << result << "руб." << endl;
};

