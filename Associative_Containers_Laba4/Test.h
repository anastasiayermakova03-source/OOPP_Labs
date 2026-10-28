#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "ThematicBlock.h"
using namespace std;

class Test
{
	string idTest;
	string nameTest;
	int totalScore;
	vector<ThematicBlock> blocks;

public:
	Test(string id, string name, vector<ThematicBlock> bl) : idTest(id), nameTest(name), blocks(bl) {
		totalScore = calculateScore();
	};
	string getIdTest() const {
		return idTest;
	}
	string getNameTest() const {
		return nameTest;
	}
	int getScore() const {
		return totalScore;
	}
	string getThematicBlocks() const {
		return blocks[0].getTopic();
	}
	vector<ThematicBlock>& getBlocks(){
		return blocks;
	}
	void setNameTest(string name) {
		nameTest = name;
	}
	void setScore(int sc) {
		totalScore = sc;
	}
	int getAllQuestions() const {
		int sum = 0;
		for (int i = 0; i < blocks.size(); ++i)
			sum += blocks[i].getCountQuestions();
		return sum;
	}
	int calculateScore() {
		int res = 0;
		vector<ThematicBlock>::const_iterator it;
		for (it = blocks.begin(); it != blocks.end(); ++it)
			res += it->getMaxRes();
		return res;
	}
	void addThematicBlock(ThematicBlock temp) {
		blocks.push_back(temp);
	}
	void printTest() const {
		cout << " реяр \"" << nameTest << "\"\n";
		vector<ThematicBlock>::const_iterator it;
		for (it = blocks.begin(); it != blocks.end(); ++it)
			it->printThematicBlock();
	}
};


