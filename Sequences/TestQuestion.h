#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>

using namespace std;

class TestQuestion
{
	string text;
	int maxScore;

public:
	TestQuestion() : text("нет текста"), maxScore(0) {}
	TestQuestion(string t, int m) {
		text = t;
		maxScore = m;
	}
	string getText() const {
		return text;
	}
	int getMaxScore() const {
		return maxScore;
	}
	void setText(string t) {
		text = t;
	}
	void setMaxScore(int m) {
		maxScore = m;
	}
	virtual int calculateScore(string s) = 0;
	virtual void printTestQuestion() const {
		cout << getText() << endl;
	};
	virtual void saveToFile(ofstream& file) const {}
	virtual ~TestQuestion() {};
};


class ChoiceQuestion : public TestQuestion
{
	array<string, 4> options;
	int numRight;

public:
	ChoiceQuestion() : TestQuestion() {
		options.fill("");
		numRight = 0;
	}
	ChoiceQuestion(string t, int m, array<string, 4> opt, int num) : TestQuestion(t, m) {
		numRight = num;
		options = opt;
	}
	void getOptions() const {
		for (int i = 0; i < 4; ++i)
			cout << i + 1 << ") " << options[i] << endl;
	}
	int getNumRight() const {
		return numRight;
	}
	void setNumRight(int n) {
		numRight = n;
	}
	void setOptions(const array<string, 4> opt) {
		options = opt;
	}
	int calculateScore(string s) override {
		if (stoi(s) == numRight)
			return getMaxScore();
		else return 0;
	};
	void printTestQuestion() const override {
		TestQuestion::printTestQuestion();
		getOptions();
	};
	void saveToFile(ofstream& file) const override {
		file << "1;" << getText() << ";" << getMaxScore() << ";";
		for (int i = 0; i < 4; ++i) {
			file << options[i] << ";";
		}
		file << numRight << "\n";
	}
};


class DefQuestion : public TestQuestion
{
	string corAnswer;

public:
	DefQuestion() : TestQuestion() {}
	DefQuestion(string t, int m, string ans) : TestQuestion(t, m) {
		corAnswer = ans;
	}
	string getCorAnswer() const {
		return corAnswer;
	}
	void setCorAnswer(string ans) {
		corAnswer = ans;
	}
	int calculateScore(string s) override {
		if (s == corAnswer)
			return getMaxScore();
		else return 0;
	};
	void printTestQuestion() const override {
		TestQuestion::printTestQuestion();
	};
	void saveToFile(ofstream& file) const override {
		file << "2;" << getText() << ";" << getMaxScore() << ";" << corAnswer << "\n";
	}
};


