#pragma once
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include "FileManager.h"

using namespace std;

class Testing
{
	unordered_map<string, Student> students;
	map<string, Test> tests;
	multimap<string, TestResult> results;

public:
	int load() { return FileManager::loadData(students, tests, results); }
	void save() { FileManager::saveData(students, tests, results); }

	bool hasStudents() const { return !students.empty(); }
	bool hasTests() const { return !tests.empty(); }
	bool hasResults() const { return !results.empty(); }

	const unordered_map<string, Student>& getStudents() const { return students; }
	const map<string, Test>& getTests() const { return tests; }

	bool addStudent(const Student& st) {
		return students.emplace(st.getId(), st).second;
	}

	bool removeStudent(const string& id) {
		return students.erase(id) > 0;
	}

	const Student* findStudent(const string& id) const {
		unordered_map<string, Student>::const_iterator it = students.find(id);
		if (it == students.end()) return nullptr;
		return &it->second;
	}

	bool addTest(const Test& t) {
		return tests.emplace(t.getIdTest(), t).second;
	}

	bool removeTest(const string& id) {
		return tests.erase(id) > 0;
	}

	Test* findTest(const string& id) {
		map<string, Test>::iterator it = tests.find(id);
		if (it == tests.end()) return nullptr;
		return &it->second;
	}

	const Test* findTest(const string& id) const {
		map<string, Test>::const_iterator it = tests.find(id);
		if (it == tests.end()) return nullptr;
		return &it->second;
	}

	// Создание через фабричный конструктор TestResult::create()
	bool addResult(const string& stId, const string& tId, int score) {
		unordered_map<string, Student>::iterator curSt = students.find(stId);
		map<string, Test>::iterator curT = tests.find(tId);
		if (curSt == students.end() || curT == tests.end()) {
			cout << "Студент или тест не найден" << endl;
			return false;
		}
		try {
			results.emplace(stId, TestResult::create(curSt->second.getName(), curT->second.getTitle(), score));
		}
		catch (const exception& e) {
			cout << "Результат не добавлен: " << e.what() << endl;
			return false;
		}
		curSt->second.addMyResult(curT->first);
		return true;
	}

	void printStudents() const {
		for (unordered_map<string, Student>::const_iterator it = students.begin(); it != students.end(); ++it) {
			cout << "[" << it->second.getId() << "] " << it->second.getName() << endl;
		}
	}

	void printTests() const {
		for (map<string, Test>::const_iterator it = tests.begin(); it != tests.end(); ++it)
			it->second.printTest();
	}

	void printResults() const {
		for (multimap<string, TestResult>::const_iterator it = results.begin(); it != results.end(); ++it) {
			it->second.printTestResult();
			cout << endl;
		}
	}
};