/* Разработать набор классов (минимум 5 классов, связи между классами: агрегация, композиция, наследование) 
по предметной области «Музыкальный магазин». Функционал программы должен позволить cобрать заказ.
Сгенерировать минимум пять типов исключительных ситуаций. Реализовать перенаправление 
исключительных ситуаций. Сгенерировать минимум одну исключительную ситуацию с оператором new.
Создать исключительную ситуацию в конструкторе и продемонстрировать вызов конструкторов
и деструкторов. Задать собственную функцию завершения. Создать собственный (пользовательский) 
класс исключения, сгенерировать исключение этого типа и обработать его. 
*/

#include "MusicStore.h"
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
            " 1.Добавить товар\n 2.Добавить услугу\n" << " 3.Просмотреть список товаров\n" <<
            " 4.Просмотреть список услуг\n" << " 5.Рассчитать стоимость чека\n" << " 6.Просмотреть корзину\n"
            " 0.Выход из программы\n" << "\tВаш выбор: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(10000, '\n');
            cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
        }
        else if (choice <= 0 || choice > 6)
            cout << "Такого варианта нет :(" << endl;
        else break;
    }
    return choice;
}

template <class T>
void ShowList(const Array<T>& other) {
    cout << "\n =============== ПРОСМОТР СПИСКА ТОВАРОВ ИЛИ УСЛУГ ================" << endl;
    int size = other.getSize();
    cout << "| № |" << setw(40) << "Наименование" << "|" << setw(5) << "Цена" << "|" << setw(15) << "Кол-во единиц" << "|" << endl;
    for (int i = 0; i < size; i++) {
        cout << "|" << setw(3) << i + 1 << "|";
        other.getEl(i).Show();
    }
    cout << " ================================================================== " << endl;
}

void myTerminate() {
    cout << "Вызвана собственная функция завершения для необработанного исключения\n";
    exit(-1);
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    set_terminate(myTerminate);

    switch (menu0()) {
    case 2: cout << "\nВыход из программы...";
        return 0;
    case 1: {

        string file1 = "instruments.txt";
        string file2 = "musicservices.txt";
        ifstream InInst(file1);
        ifstream InServ(file2);

        try {
            if (!InInst.is_open()) {
                throw ExcFile("Не получилось открыть файл", "Cистема", file1);
            }
            if (!InServ.is_open()) {
                throw ExcFile("Не получилось открыть файл", "Cистема", file2);
            }
        }
        catch (const ExcFile& ex) {
            cout << "\nПрограмма завершается..." << endl;
            return -1;
        }

        Array<ProductList> inst;
        inst.loadFile(InInst);
        Array<ServiceList> serv;
        serv.loadFile(InServ);

        Customer CurrentUser;
        string username, usercard;
        cout << "\n| Введите имя клиента: ";
        cin >> username; CurrentUser.setName(username);
        cout << "| Введите номер карты: ";
        cin >> usercard; CurrentUser.setCard(usercard);

        Order userOrder(CurrentUser);
        int number;
        while (true) {
            try {
                switch (menu()) {
                case 1: {
                    ShowList<ProductList>(inst);
                    while (true) {
                        cout << "| Введите номер товара: ";
                        cin >> number;
                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
                        }
                        else if (number <= 0 || inst.getSize() < number)
                            cout << "Такого товара нет :(" << endl;
                        else { 
                            cout << "--> Товар [" << inst.getEl(number - 1).getProduct()
                            << "] успешно добавлен в корзину\n";
                            break; 
                        }
                    }
                    ProductList add = inst.getEl(number - 1);
                    userOrder.AddProduct(add);
                    break;
                }
                case 2: {
                    ShowList<ServiceList>(serv);
                    while (true) {
                        cout << "| Введите номер услуги: ";
                        cin >> number;
                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << " !!! Ошибка. Пожалуйста, введите число !!! " << endl;
                        }
                        else if (number <= 0 || serv.getSize() < number)
                            cout << "Такой услуги нет :(" << endl;
                        else {
                            cout << "--> Услуга [" << serv.getEl(number - 1).getProduct()
                            << "] успешно добавлена в корзину\n";
                            break;
                        }
                    }
                    ServiceList add = serv.getEl(number - 1);
                    userOrder.AddService(add);
                    break;
                }
                case 3: ShowList<ProductList>(inst); break;
                case 4: ShowList<ServiceList>(serv); break;
                case 5: userOrder.CalculatePrice(); break;
                case 6: userOrder.ShowOrder(); break;
                case 0: cout << " Выход из программы...";
                    return 0;
                }
            }

            catch (const ExcPrice& ex) {
                cout << "Попробуйте ввести положительное значение чуть позже" << endl;
            }
            catch (const ExcCount& ex) {
                cout << "Попробуйте ввести положительное значение чуть позже" << endl;
            }
            catch (const ExcCountAvail& ex) {
                cout << "Попробуйте ввести удовлетворительное значение чуть позже" << endl;
            }
            catch (const ExcMemory& ex) {
                cout << "Попробуйте создать объект чуть позже" << endl;
                throw;
            }
            catch (const ExcOrder& ex) {
                cout << "Попробуйте для начала добавить товары или услуги в корзину, для рассчета ее стоимости это просто необходимо" << endl;
            }
        }
    }
    }
}