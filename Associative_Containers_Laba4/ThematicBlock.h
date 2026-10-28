#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "TestQuestion.h"
using namespace std;

class ThematicBlock
{
	string topic;
	int maxRes;
	vector<shared_ptr<TestQuestion>> questions;

public:
	ThematicBlock() : topic("Темы нет") {}
	ThematicBlock(string t, vector<shared_ptr<TestQuestion>> q) : topic(t), questions(q) {
		maxRes = calculateMaxRes();
	}
	string getTopic() const {
		return topic;
	}
	int getCountQuestions() const {
		return questions.size();
	}
	vector<shared_ptr<TestQuestion>> getQuestions() const {
		return questions;
	}
	int getMaxRes() const {
		return maxRes;
	}
	void addQuestions(shared_ptr<TestQuestion> temp) {
		questions.push_back(temp);
		maxRes = calculateMaxRes();
	}
	int calculateMaxRes() {
		int maxRes = 0;
		vector<shared_ptr<TestQuestion>>::const_iterator it;
		for (it = questions.begin(); it != questions.end(); ++it)
			maxRes += (*it)->getMaxScore();
		return maxRes;
	}
	void printThematicBlock() const {
		vector<shared_ptr<TestQuestion>>::const_iterator it;
		cout << " Тематический блок [" << topic << "]" << endl;
		for (it = questions.begin(); it != questions.end(); ++it)
			(*it)->printTestQuestion();
	}
};


