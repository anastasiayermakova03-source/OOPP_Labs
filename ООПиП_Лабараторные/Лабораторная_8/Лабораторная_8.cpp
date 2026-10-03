/*Разработать набор классов(минимум 5) по теме «Розничная продажа товаров и услуг».
Корректно реализовать связи между классами. Использовать smart - указатели для создания программы
учета продаваемых и закупаемых товаров.Реализовать механизм транзакций, который позволит
откатывать изменения, если данные о товаре введены неверно(например, цена имеет некорректное значение).
В разработанном наборе классов должен быть хотя бы один шаблонный класс.Все классы должны иметь методы 
получения и установки значений полей.Программа должна обеспечивать вывод информации об осуществленных покупках
и итоговой сумме только по запросу клиента.Использовать конструктор с параметрами, конструктор без
параметров, конструктор копирования, деструктор.*/

#include "Product.h"
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
            " 1. Добавить товар в список\n 2. Редактировать товар\n" << " 3. Просмотреть список товаров\n" <<
            " 4. Просмотреть мою корзину\n" << " 5. Рассчитать стоимость чека\n" <<
            " 0. Выход из программы\n" << "\tВаш выбор: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
        }
        else if (choice < 0 || choice > 5)
            cout << "Такого варианта нет :(" << endl;
        else break;
    }
    return choice;
}

void ShowInventary(Inventary<shared_ptr<Product>> inv) {
    cout << "\n ===== ПРОСМОТР СПИСКА ТОВАРОВ НА СКЛАДЕ =====" << endl;
    int size = inv.getKol();
    cout << "| № |" << setw(20) << "Наименование" << "|" << setw(15) << "Кол-во единиц" << "|" << setw(5) <<  "Цена" << "|" << endl;
    for (int i = 0; i < size; i++) {
        cout << "|" << setw(3) << i + 1 << "|";
        inv.printInv(i);
    }
    cout << " ============================================== " << endl;
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    switch (menu0()) {
    case 2: cout << "\nВыход из программы...";
        return 0;
    case 1: {
        system("cls");
        Customer currentUser;
        string name;
        cin.ignore();
        cout << "| Введите имя пользователя: ";
        getline(cin, name);  currentUser.setName(name);

        ifstream InProduct("product.txt");
        Inventary<shared_ptr<Product>> inventary;
        inventary.loadFile(InProduct);
        InProduct.close();

        int choice;
        while (true) {
            switch (menu()) {
            case 1: {
                ShowInventary(inventary);

                while (true) {
                    cout << "| Введите номер товара: ";
                    cin >> choice;
                    cin.ignore();
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
                    }
                    else if (choice > inventary.getKol() || choice < 1)
                        cout << "Такого варианта нет :(" << endl;
                    else break;
                }
                auto product = inventary.getProduct(choice - 1);
                int count;
                while (true) {
                    cout << "| Введите количество единиц товара: ";
                    cin >> count;
                    cin.ignore();
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << " !!! Ошибка. Пожалуйста, введите другое !!! " << endl;
                    }
                    else if (count > product->getCount() || count < 1)
                        cout << "Такого количества товара на складе нет. Доступно только " << product->getCount() << endl;
                    else {
                        product->setCount(product->getCount() - count);
                        Purchase temp(product, count);
                        currentUser.addPurchase(temp);
                        cout << " Товар успешно добавлен в вашу корзину" << endl;
                        break;
                    }
                }
                break;
            }
            case 2: {
                ShowInventary(inventary);
                while (true) {
                    cout << "| Введите номер товара: ";
                    cin >> choice;
                    cin.ignore();
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
                    }
                    else if (choice > inventary.getKol() || choice < 1)
                        cout << "Такого варианта нет :(" << endl;
                    else break;
                }
                Transaction tr;
                auto pr = inventary.getProduct(choice - 1);
                tr.begin(*pr);
                try {
                    double newPrice;
                    int newCount;
                    cout << "| Введите новую цену для товара [" << pr->getName() << "]: ";
                    cin >> newPrice;
                    cout << "| Введите количество единиц на складе: ";
                    cin >> newCount;
                    pr->setPrice(newPrice);
                    pr->setCount(newCount);
                    tr.commit();
                    cout << " Товар [" << pr->getName() << "] успешно отредактирован\n";
                }
                catch (...) {
                    tr.rollback(*pr);
                    cout << " Ошибка редактирования. Изменения не были применены\n";
                }
                break;
            }
            case 3: ShowInventary(inventary); break;
            case 4: currentUser.showPurchase(); break;
            case 5: currentUser.printCheck(); break;
            case 0: cout << "Выход из программы..." << endl;
                return 0;
            }
        }
    }
    }
}