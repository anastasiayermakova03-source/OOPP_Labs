#pragma once
#include <memory>
#include "TestQuestion.h"

// Фабричный метод - какой пустой вопрос создать

class TestQuestionCreator
{
public:
	virtual ~TestQuestionCreator() = default;
	virtual unique_ptr<TestQuestion> createQuestion() const = 0;
};

class ChoiceQuestionCreator : public TestQuestionCreator
{
public:
	unique_ptr<TestQuestion> createQuestion() const override {
		return make_unique<ChoiceQuestion>();
	}
};

class DefQuestionCreator : public TestQuestionCreator
{
public:
	unique_ptr<TestQuestion> createQuestion() const override {
		return make_unique<DefQuestion>();
	}
};
