#include "String.h"
#include <iostream>
#include <cstring>
using namespace std;

String::String() {
	size = 0;
	str = new char[1];
	str[0] = '\0';
}
String::String(const char* other) {
	size = (int)strlen(other);
	str = new char[size + 1];
	strcpy(str, other);
}
String::String(const String& other) {
	size = other.size;
	str = new char[size + 1];
	strcpy(str, other.str);
}

String::~String() {
	delete[] str;
	size = 0;
}

const char* String::Str() const { return str; }
int String::SizeStr() const { return size; }

String& String::operator=(const String& other) {
	if (&other == this) 
		return *this;

	size = other.size;
	delete[] str;
	str = new char[size + 1];
	strcpy(str, other.str);
	return *this;
}

String& String::operator+=(const String& other) {
	int NewSize = other.size + size+1;
	char* buffer = new char[NewSize];
	strcpy(buffer, str);
	strcat(buffer, other.str);

	delete[] str;
	str = new char[NewSize];
	strcpy(str, buffer);
	size = NewSize-1;
	return *this;
}

ostream& operator<<(ostream& out, const String& other) {
	out << other.str;
	return out;
}

istream& operator>>(istream& in, String& other) {
	char buffer[1024];
	in >> buffer;
	other.size = strlen(buffer);
	delete[] other.str;
	other.str = new char[other.size + 1];
	strcpy(other.str, buffer);
	return in;
}
