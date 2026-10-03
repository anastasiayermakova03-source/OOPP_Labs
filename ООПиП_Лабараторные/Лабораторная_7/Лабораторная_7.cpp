#include "Market.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>
#include <cstdlib>
using namespace std;

int menu0() {
    int choice;
    while (true) {
        cout << "\n ===== МЕНЮ нулевого уровня =====\n" <<
            " 1. Начать работу\n 2. Завершить программу\n" << "\tВаш выбор: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
        }
        else if (choice != 1 && choice != 2)
            cout << "Такого варианта нет :(" << endl;
        else break;
    }
    return choice;
}

int menu() {
    int choice;
    while (true) {
        cout << "\n ======== МЕНЮ ========\n" <<
            " 1. Добавить товар\n 2. Добавить услугу\n" <<
            " 3. Просмотреть корзину\n" << " 4. Рассчитать стоимость корзины\n" <<
            " 0. Выход из программы\n" << "\tВаш выбор: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
        }
        else if (choice < 0 || choice > 4)
            cout << "Такого варианта нет :(" << endl;
        else break;
    }
    return choice;
}

void ShowPr(Array<ProductList> pr) {
    cout << "\n ===== ПРОСМОТР СПИСКА ТОВАРОВ =====" << endl;
    int size = pr.getSize();
    cout << "| № |" << setw(20) << "Наименование" << "|" << setw(5) << "Цена" << "|" << endl;
    for (int i = 0; i < size; i++) {
            cout << "|" << setw(3) << i + 1 << "|";
            pr.getEl(i).Show();
        }
    cout << " ===================================" << endl;
}

void ShowSr(Array<ServiceList> sr) {
    cout << "\n ===== ПРОСМОТР СПИСКА УСЛУГ =====" << endl;
    int size = sr.getSize();
    cout << "| № |" << setw(20) << "Наименование" << "|" << setw(5) << "Цена" << "|" << setw(5) << "Время" << "|" << endl;
    for (int i = 0; i < size; i++) {
        cout << "|" << setw(3) << i + 1 << "|";
        sr.getEl(i).Show();
    }
    cout << " =================================" << endl;
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    switch (menu0()) {
    case 2: cout << "\nВыход из программы...";
        return 0;
    case 1:
        system("cls");
        Customer currentUser; 
        string name, card;
        cin.ignore();
        cout << "| Введите имя пользователя: ";
        getline(cin, name);  currentUser.setName(name);
        cout << "| Введите номер карты: ";
        getline(cin, card);  currentUser.setCard(card);
        Order order(currentUser);

        ifstream InProd("products.txt");
        Array<ProductList> products;
        products.loadFile(1, InProd);
        InProd.close();

        ifstream InServ("services.txt");
        Array<ServiceList> services;
        services.loadFile(2, InServ);
        InServ.close();

        int choice;
        while (true) {
            switch (menu()) {
            case 1:
                ShowPr(products);
                while (true) {
                    cout << "| Введите номер товара: ";
                    cin >> choice;
                    cin.ignore();
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
                    }
                    else if (choice > products.getSize() || choice < 1)
                        cout << "Такого варианта нет :(" << endl;
                    else break;
                }
                order.AddProduct(products.getEl(choice-1));
                cout << " Товар [" << choice << "] успешно добавлен в корзину\n";
                break;
            case 2:
                ShowSr(services);
                while (true) {
                    cout << "| Введите номер услуги: ";
                    cin >> choice;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
                    }
                    else if (choice > services.getSize() || choice < 1)
                        cout << "Такого варианта нет :(" << endl;
                    else break;
                }
                order.AddService(services.getEl(choice-1));
                cout << " Услуга [" << choice << "] успешно добавлена в корзину\n";
                break;
            case 3: order.ShowOrder(); break;
            case 4: order.CalculatePrice(); break;
            case 0: cout << "Выход из программы..." << endl;
                return 0;
            }
        }
    }
}