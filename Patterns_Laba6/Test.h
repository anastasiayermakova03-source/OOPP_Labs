#pragma once
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>
#include "TestQuestion.h"

using namespace std;

class Test
{
	string idTest;
	string title;
	int totalScore = 10;
	vector<unique_ptr<TestQuestion>> questions;

public:
	Test(string id, string name, const vector<const TestQuestion*>& q = {})
		: idTest(move(id)), title(move(name))
	{
		questions.reserve(q.size());
		for (const TestQuestion* item : q) {
			if (item != nullptr) {
				questions.push_back(item->clone());
			}
		}
	}

	Test(string id, string name, const vector<TestQuestion*>& q)
		: idTest(move(id)), title(move(name))
	{
		questions.reserve(q.size());
		for (const TestQuestion* item : q) {
			if (item != nullptr) {
				questions.push_back(item->clone());
			}
		}
	}

	Test(const Test& other)
		: idTest(other.idTest), title(other.title), totalScore(other.totalScore)
	{
		questions.reserve(other.questions.size());
		for (const auto& item : other.questions) {
			if (item != nullptr) {
				questions.push_back(item->clone());
			}
		}
	}

	Test& operator=(const Test& other) {
		if (this == &other) {
			return *this;
		}
		idTest = other.idTest;
		title = other.title;
		totalScore = other.totalScore;
		questions.clear();
		questions.reserve(other.questions.size());
		for (const auto& item : other.questions) {
			if (item != nullptr) {
				questions.push_back(item->clone());
			}
		}
		return *this;
	}

	Test(Test&&) noexcept = default;
	Test& operator=(Test&&) noexcept = default;
	~Test() = default;

	string getIdTest() const {
		return idTest;
	}
	string getTitle() const {
		return title;
	}
	int getScore() const {
		return totalScore;
	}
	const vector<unique_ptr<TestQuestion>>& getTestQuestions() const {
		return questions;
	}
	void setTitle(string name) {
		title = move(name);
	}

	void addTestQuestion(const TestQuestion& q) {
		if (questions.size() < 5) {
			questions.push_back(q.clone());
		}
		else {
			cout << "Нельзя добавить больше 5 вопросов" << endl;
		}
	}

	void deleteTestQuestion(int ind) {
		if (ind < 1 || ind > static_cast<int>(questions.size())) {
			return;
		}
		questions.erase(questions.begin() + (ind - 1));
	}

	void printTest() const {
		cout << " ТЕСТ \"" << title << "\"\n";
		int ind = 1;
		for (const auto& q : questions) {
			if (q != nullptr) {
				cout << "[" << ind++ << "] ";
				q->printTestQuestion();
				cout << endl;
			}
		}
	}

	void saveTest(ostream& file) const {
		file << idTest << ";" << title << ";" << questions.size() << endl;
		for (const auto& q : questions) {
			if (q != nullptr) {
				q->saveToFile(file);
			}
		}
	}
};