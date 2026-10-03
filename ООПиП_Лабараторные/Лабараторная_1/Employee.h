#pragma once
#include <iostream>
#include <string>

using namespace std;

class Employee
{
private:
    string name;
    int number;
    static int count;

    static Employee* massiv;
    static int size;
    static int capacity;

public:
    Employee();
    Employee(string n);
    Employee(const Employee& cp);
    ~Employee();

    string getName() const;
    int getNumber() const;
    void setName(const string& n);
    void setNumber(const int& num);

    static void startEmp();
    static void addEmp();
    static void searchNum();
    static void printEmp();
    static void deleteEmp();
    static void saveToFile(const string& file);
    static void loadFromFile(const string& filename);

};