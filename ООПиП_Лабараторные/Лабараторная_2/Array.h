#pragma once
#include <iostream>
using namespace std;

class Array
{
private:
	int* intArray = nullptr;
	double* doubleArray = nullptr;
	char* charArray = nullptr;
	int size;
	int type;

public:
	Array(int s, int t);
	Array();
	Array(const Array& other);
	~Array();

	void CopyData(const Array& other);
	void Fill();
	void UnionMassiv(const Array& other);
	void IntersectionMassiv(const Array& other);
	std::ostream& PrintRow(std::ostream& stream) const;   // Вывод строкой 
	std::ostream& PrintCol(std::ostream& stream) const;    // Вывод столбцом 
};