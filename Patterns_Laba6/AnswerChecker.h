#pragma once
#include <cctype>
#include <string>
#include "TestQuestion.h"

class AnswerChecker {
public:
	virtual ~AnswerChecker() = default;
	virtual int checkAnswer(const TestQuestion& question, const std::string& userAnswer) const = 0;
};

// проверка для режима "Экзамен"
class ExamAnswerChecker : public AnswerChecker {
private:
	static bool parseChoiceNumber(const std::string& s, int& outNum) {
		if (s.empty()) {
			return false;
		}
		try {
			size_t pos = 0;
			int val = std::stoi(s, &pos);
			if (pos != s.size()) {
				return false;
			}
			outNum = val;
			return true;
		}
		catch (...) {
			return false;
		}
	}

public:
	int checkAnswer(const TestQuestion& question, const std::string& userAnswer) const override {
		if (const auto* choice = dynamic_cast<const ChoiceQuestion*>(&question)) {
			int chosen = 0;
			if (parseChoiceNumber(userAnswer, chosen) && chosen == choice->getNumRight()) {
				return question.getMaxScore();
			}
			return 0;
		}

		if (const auto* def = dynamic_cast<const DefQuestion*>(&question)) {
			if (userAnswer == def->getCorAnswer()) {
				return question.getMaxScore();
			}
			return 0;
		}

		return 0;
	}
};

// проверка для режима "Тренировка"
class PracticeAnswerChecker : public AnswerChecker {
private:
	static std::string trimAndLower(std::string s) {
		while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
			s.erase(s.begin());
		}
		while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
			s.pop_back();
		}
		for (char& ch : s) {
			ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
		}
		return s;
	}

	static bool parseChoiceNumber(const std::string& s, int& outNum) {
		std::string cleaned = trimAndLower(s);
		if (cleaned.empty()) {
			return false;
		}
		try {
			size_t pos = 0;
			int val = std::stoi(cleaned, &pos);
			if (pos != cleaned.size()) {
				return false;
			}
			outNum = val;
			return true;
		}
		catch (...) {
			return false;
		}
	}

public:
	int checkAnswer(const TestQuestion& question, const std::string& userAnswer) const override {
		if (const auto* choice = dynamic_cast<const ChoiceQuestion*>(&question)) {
			int chosen = 0;
			if (parseChoiceNumber(userAnswer, chosen) && chosen == choice->getNumRight()) {
				return question.getMaxScore();
			}
			return 0;
		}

		if (const auto* def = dynamic_cast<const DefQuestion*>(&question)) {
			if (trimAndLower(userAnswer) == trimAndLower(def->getCorAnswer())) {
				return question.getMaxScore();
			}
			return 0;
		}

		return 0;
	}
};