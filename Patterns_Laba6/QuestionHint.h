#pragma once
#include <string>
#include "TestQuestion.h"

class QuestionHint {
public:
    virtual ~QuestionHint() = default;
    virtual bool getHint(const TestQuestion& question, std::string& outHint) const = 0;
};

// В экзамене подсказки недоступны
class ExamQuestionHint : public QuestionHint {
public:
    bool getHint(const TestQuestion&, std::string& outHint) const override {
        outHint.clear();
        return false;
    }
};

// В тренировке подсказка формируется по типу вопроса
class PracticeQuestionHint : public QuestionHint {
public:
    bool getHint(const TestQuestion& question, std::string& outHint) const override {
        if (const auto* choice = dynamic_cast<const ChoiceQuestion*>(&question)) {
            (void)choice;
            outHint = "Подсказка: отбрось заведомо неверные варианты и сверься с базовым синтаксисом/правилом.";
            return true;
        }
        if (const auto* def = dynamic_cast<const DefQuestion*>(&question)) {
            (void)def;
            outHint = "Подсказка: сформулируй ответ одним точным термином или ключевым словом без лишних слов.";
            return true;
        }
        outHint = "Подсказка: внимательно перечитай условие вопроса.";
        return true;
    }
};