#pragma once

#include "ExamQuestion.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamQuestion : ExamQuestionT<ExamQuestion>
    {
        ExamQuestion();

        hstring Text();
        void Text(hstring const& value);

        winrt::ProPractice::ExamQuestionType Type();
        void Type(winrt::ProPractice::ExamQuestionType const& value);

        winrt::Windows::Foundation::Collections::IVector<winrt::ProPractice::ExamAnswer> Answers();

    private:
        hstring _text;
        ExamQuestionType _type;
        winrt::Windows::Foundation::Collections::IVector<winrt::ProPractice::ExamAnswer> _answers = nullptr;
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamQuestion : ExamQuestionT<ExamQuestion, implementation::ExamQuestion>
    {
    };
}
