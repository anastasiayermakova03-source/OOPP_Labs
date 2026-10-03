// 11. Разработать набор классов(минимум 5 классов, связи между классами : агрегация, композиция, наследование) 
// по предметной области «Магазин детских игрушек».Функционал программы должен позволить собрать заказ.

#include <iostream>
#include <windows.h>
#include <string>
#include <cctype>
#include <algorithm>
#include "ToysShop.h"
using namespace std;

static const int mas = 8;

int menu() {
    cout << "\n ---- МЕНЮ ----- " << endl;
    cout << "1) просмотр каталога\n" <<
        "2) поиск в каталоге\n" <<
        "3) сортировка каталога по цене\n" <<
        "4) добавить товар в корзину\n" <<
        "5) посмотреть корзину\n" <<
        "6) редактировать товары в корзине\n" <<
        "7) удалить товар из корзины\n" <<
        "8) очистить всю корзину\n" <<
        "9) рассчитать общую стоимость корзины\n" << 
        "10) закончить\n";
    int choice_menu;
    while (true) {
        cout << "ВЫБЕРИТЕ: ";
        cin >> choice_menu;
        if (cin.good()) {
            if (choice_menu > 10 || choice_menu < 1)
                cout << "Команда [" << choice_menu << "] отсутствует" << endl;
            else 
            break;
        }
        cin.clear();
        cout << "Некорректный ввод" << endl;
        cin.ignore(100, '\n');
    }
    return  choice_menu;
}


int survey(Order* userOrder) {
    if (userOrder->getCurrent() == 0) {
        cout << "\nКорзина пуста\n";
        return -1;
    }
    userOrder->printOrder();
    int item;
    while (true) {
        cout << "\nВведите номер позиции: ";
        cin >> item;
        if (cin.good()) {
            if (item > userOrder->getCurrent() || item < 1)  
                cout << "Позиция [" << item << "] отсутствует" << endl;
            else
                return item;
        }
        cin.clear();
        cout << "Некорректный ввод" << endl;
        cin.ignore(100, '\n');
    }
}


bool comparePrice(Toy* first, Toy* second) {  // компаратор-функция
    return first->getPrice() < second->getPrice();
}


string toLow(string n) {
    string res = n;
    for (int i = 0; i < n.length(); i++) {
        res[i] = tolower(n[i]);
        if (n[i] >= 'А' && n[i] <= 'Я') {
            res[i] = n[i] + 32;
        }
        else if (n[i] == 'Ё') {
            res[i] = 'ё';
        }
    }
    return res;
}


bool searchToy(Toy* Catalog[mas]) {
    cout << "\n ---- ПОИСК ---- \n";
    string name;
    cin.ignore(100, '\n');
    while (true) {
        cout << "Введите название товара:";
        getline(cin,name);
        if (cin.good() && !name.empty()) break;
        cin.clear();
        cout << "Некорректный ввод" << endl;
    }

    bool result = false;
    for (int i = 0; i < mas; i++) {
        if (toLow(name) == toLow(Catalog[i]->getName())) {
            cout << string(72,'-') << endl << "|";
            Catalog[i]->print();
            cout << "|" << endl;
            cout << string(72, '-') << endl;
            result = true;
        }
    }
    if (!result) cout << "Товар [" << name << "] отсутствует в каталоге\n";
    return result;
}


void printCatalog(Toy* Catalog[mas]) {
    if (mas == 0) {
        cout << "Каталог пуст\n";
        return;
    }
    cout.width(50);
    cout << " --- КАТАЛОГ МАГАЗИНА --- " << endl;
    cout << string(76, '-') << endl;
    cout << "|" << setw(3) << "ID" << "|" << setw(30) << "Наименование" << "|" << setw(7) << "Цена" << "|" << setw(15) << "Свойство 1" << "|"
         << setw(15) << "Свойство 2" << "|" << endl;
    cout << string(76, '-') << endl;
    for (int i = 0; i < mas; i++) {
        cout << "|" << setw(3) << i+1 << "|";
        Catalog[i]->print();
        cout << "|" << endl;
    }
    cout << string(76, '-') << endl;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Car car1 = Car("Машинка на пульте управления", 205.8, "красная", "внедорожник");
    Car car2 = Car("Игрушечный грузовик", 33.5, "желтый", "грузовик");
    Car car3 = Car("Набор машинок", 59.9, "разноцветные", "гоночные");
    Doll doll1 = Doll("Барби с домом", 50.5, "Барби", 1);
    Doll doll2 = Doll("Набор фигурок HelloKitty", 24.45, "HelloKitty", 0);
    Constructor constr1 = Constructor("Замок Lego", 220.32, 1066, "средний уровень");
    Constructor constr2 = Constructor("Цветы Lego", 106.45, 543, "легкий уровень");
    Constructor constr3 = Constructor("Пирамида", 79.9, 120, "легкий уровень");

    Toy* myCatalog[mas];
    myCatalog[0] = &car1;
    myCatalog[1] = &car2;
    myCatalog[2] = &car3;
    myCatalog[3] = &doll1;
    myCatalog[4] = &doll2;
    myCatalog[5] = &constr1;
    myCatalog[6] = &constr2;
    myCatalog[7] = &constr3;

    Order userOrder;

    while (true) {
        switch (menu()) {
        case 1:
            printCatalog(myCatalog); break;
        case 2:
            searchToy(myCatalog); break;  // здесь ошибка + добавить вывод корзины
        case 3:
            cout << " ---- CОРТИРОВКА ---- \n";
            sort(myCatalog, myCatalog + mas, comparePrice);
            printCatalog(myCatalog); break;
        case 4: {
            printCatalog(myCatalog);
            int choice;
            while (true) {
                cout << "\nВведите номер позиции: ";
                cin >> choice;
                if (cin.good()) {
                    if (choice > mas || choice < 1)
                        cout << "Позиция [" << choice << "] отсутствует" << endl;
                    else
                        break;
                }
                cin.clear();
                cout << "Некорректный ввод" << endl;
                cin.ignore(100, '\n');
            }
            int count;
            while (true) {
                cout << "Введите кол-во товаров: ";
                cin >> count;
                if (count >= 1) break;
                if (cin.fail() || count < 1) {
                    cout << "Некорректный ввод\n";
                    cin.clear();
                    cin.ignore(10, '\n');
                }
            }
            userOrder.addOrderOne(OrderOne(myCatalog[choice - 1], count));
            break;
        }
        case 5: {
            if (userOrder.getCurrent() == 0) {
                cout << "\nКорзина пуста\n";
                break;
            }
            userOrder.printOrder();
            break;
        }
        case 6: {
            int survey6 = survey(&userOrder);
            if (survey6 == -1)  break;
            else {
                userOrder.changeOrderOne(survey6);
                if (userOrder.getCount(survey6) == 0) {
                    //cout << endl << userOrder.getCount(survey6) << endl;
                    userOrder.deleteOrderOne(survey6);
                }
                break;
            }
        }
        case 7: {
              int survey7 = survey(&userOrder);
              if (survey7 == -1)  break;
              else {
                  userOrder.deleteOrderOne(survey7);
                  break;
              }
        }
        case 8:
            if (userOrder.getCurrent() == 0) {
                cout << "\nКорзина пуста\n";
                break;
            }
            userOrder.clearOrderOne(); break;
        case 9: {
            cout << "\n ---- ИТОГОВЫЙ ЧЕК ----\n";
            int sum = userOrder.totalPrice();
            if (sum == -1) break;
            cout << "Общая стоимость заказа = " << sum << " рубл." << endl; break;
        }
        case 10:
            cout << "\nВыход из программы\n";
            return 0;
        }
    }
   
}
