#pragma once
#include <iostream>
using namespace std;

class String
{
	char* str;
	int size;
public:
	String();
	String(const char* other);
	String(const String& other);
	~String();

	const char* Str() const;
	int SizeStr() const;
	String& operator=(const String& other);
	String& operator+=(const String& other);

	friend ostream& operator<<(ostream& out, const String& other);
	friend istream& operator>>(istream& in, String& other);
};

