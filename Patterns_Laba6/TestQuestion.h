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
	int maxScore = 2;

public:
	TestQuestion() : text("нет текста"), maxScore(0) {}
	TestQuestion(string t): text(t) {}
	string getText() const {
		return text;
	}
	void setText(string t) {
		text = t;
	}
	int getMaxScore() const {
		return maxScore;
	}
	virtual void printTestQuestion() const {
		cout << getText() << endl;
	};
	virtual void saveToFile(ostream& file)  {}

	virtual unique_ptr<TestQuestion> clone() const { // фабричное копирование
		return make_unique<TestQuestion>(*this);
	}
	
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
	ChoiceQuestion(string t, array<string, 4> opt, int num) : TestQuestion(t) {
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
	void setOptions(const array<string, 4>& opt) {
		options = opt;
	}
	void printTestQuestion() const override {
		TestQuestion::printTestQuestion();
		getOptions();
	};
	void saveToFile(ostream& file) override {
		file << "CHOICE;" << getText() << ";" << numRight << ";";
		for (int i = 0; i < 4; ++i) {
			file << options[i] << ";";
		}
		file << "\n";
	}
	unique_ptr<TestQuestion> clone() const override {
		return make_unique<ChoiceQuestion>(*this);
	}
};


class DefQuestion : public TestQuestion
{
	string corAnswer;

public:
	DefQuestion() : TestQuestion() {}
	DefQuestion(string t, string ans) : TestQuestion(t) {
		corAnswer = ans;
	}
	string getCorAnswer() const {
		return corAnswer;
	}
	void setCorAnswer(string ans) {
		corAnswer = ans;
	}
	void printTestQuestion() const override {
		TestQuestion::printTestQuestion();
	};
	void saveToFile(ostream& file) override {
		file << "DEF;" << getText() << ";" << corAnswer << "\n";
	}
	unique_ptr<TestQuestion> clone() const override {
		return make_unique<DefQuestion>(*this);
	}
};


