#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "TestQuestion.h"
using namespace std;

class Test
{
	string idTest;
	string title;
	int totalScore = 10;
	vector<TestQuestion*> questions;

public:
	Test(string id, string name, vector<TestQuestion*> q = {}) : idTest(id), title(name) {
		for (vector<TestQuestion*>::iterator p = q.begin(); p != q.end(); ++p)
			if (*p) questions.push_back((*p)->clone());
	};
	Test(const Test& other): idTest(other.idTest), title(other.title), totalScore(other.totalScore) {
		for (vector<TestQuestion*>::const_iterator p = other.questions.begin(); p != other.questions.end(); ++p)
			questions.push_back((*p)->clone());
	}

	string getIdTest() const {
		return idTest;
	}
	string getTitle() const {
		return title;
	}
	int getScore() const {
		return totalScore;
	}
	vector<TestQuestion*> getTestQuestions() const {
		return questions;
	}
	void setTitle(string name) {
		title = name;
	}
	void addTestQuestion(TestQuestion& q) {
		questions.push_back(&q);
	}
	void deleteTestQuestion(int ind) {
		if (ind < 1 || ind > static_cast<int>(questions.size())) return;
		delete questions[ind - 1];
		questions.erase(questions.begin() + (ind - 1));
	}
	void printTest() const {
		cout << " реяр \"" << title << "\"\n";
		int ind = 1;
		for (auto it = questions.begin(); it != questions.end(); ++it) {
			cout << "[" << ind++ << "] ";
			(*it)->printTestQuestion();
			cout << endl;
		}
	}
	void saveTest(ostream& file) const {
		file << idTest << ";" << title << ";" << questions.size() << endl;
		for (int i = 0; i < static_cast<int>(questions.size()); ++i)
			questions[i]->saveToFile(file);
	}
};


