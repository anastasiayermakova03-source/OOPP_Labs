#pragma once
#include <memory>
#include "QuestionHint.h"
#include "AnswerChecker.h"

class TestModeFactory {
public:
    virtual ~TestModeFactory() = default;
    virtual std::unique_ptr<QuestionHint> createHint() const = 0;
    virtual std::unique_ptr<AnswerChecker> createChecker() const = 0;
    virtual std::string getModeName() const = 0;
};

class PracticeModeFactory : public TestModeFactory {
public:
    std::unique_ptr<QuestionHint> createHint() const override {
        return std::make_unique<PracticeQuestionHint>();
    }
    std::unique_ptr<AnswerChecker> createChecker() const override {
        return std::make_unique<PracticeAnswerChecker>();
    }
    std::string getModeName() const override {
        return "Тренировка";
    }
};

class ExamModeFactory : public TestModeFactory {
public:
    std::unique_ptr<QuestionHint> createHint() const override {
        return std::make_unique<ExamQuestionHint>();
    }
    std::unique_ptr<AnswerChecker> createChecker() const override {
        return std::make_unique<ExamAnswerChecker>();
    }
    std::string getModeName() const override {
        return "Экзамен";
    }
};
