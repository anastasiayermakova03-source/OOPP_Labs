#pragma once
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>
#include <array>
using namespace std;

class Question
{
	int id;
	string text;
	array<string, 4> options;
	int correctNum;

public:
	Question() : text(""), id(0), correctNum(0) {}
	Question(int i, string t, array<string, 4>& opt, int n): id(i), text(t), correctNum(n) {
		for (int t = 0; t < 4; ++t)
			options[t] = opt[t];
	}

	int getId() const { return id; }
	string getText() const { return text; }
	int getCorrectNum() const { return correctNum; }
	void printQuestion() const { 
		cout << id << ". " << text << endl;
		for (int t = 0; t < 4; ++t)
			cout << t + 1 << ") " << options[t] << endl;
	}
	string saveQuestion() const {
		stringstream ss;
		ss << id << ";" << text << ";";
		for (int t = 0; t < 4; ++t)
			ss << options[t] << ";";
		ss << correctNum;
		return ss.str();
	}
	~Question() {};
};


class Test
{
	string idTest;
	string title;
	int totalScore = 10;
	vector<Question> questions;

public:
	Test() : idTest(""), title("") {}
	Test(string id, string tit, vector<Question> q) : idTest(id), title(tit), questions(q) {}

	string getIdTest() const { return idTest; }
	string getTitle() const { return title; }
	vector<Question> getQuestions() { return questions; }
	int getQuestionsCount() const { return questions.size(); }
	void addQuestion(const Question& q) { questions.push_back(q); }

	void printTest() const {
		cout << " реяр \"" << title << "\"\n";
		vector<Question>::const_iterator it;
		for (it = questions.begin(); it != questions.end(); ++it) {
			it->printQuestion();
			cout << "\n";
		}	
	}

	void saveTest(ostream& file) const {
		file << idTest << ";" << title << ";" << questions.size() << endl;
		for (int i = 0; i < static_cast<int>(questions.size()); ++i)
			file << questions[i].saveQuestion() << endl;
	}

	static string generateId() {
		return "T-" + to_string(100 + rand() % 900);
	}
};

