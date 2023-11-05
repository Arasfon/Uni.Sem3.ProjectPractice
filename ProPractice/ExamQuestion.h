#pragma once

#include "ExamQuestion.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamQuestion : ExamQuestionT<ExamQuestion>
    {
        ExamQuestion();

        hstring Text();
        void Text(hstring const& value);

        winrt::ProPractice::ExamQuestionType Type() const;
        void Type(winrt::ProPractice::ExamQuestionType const& value);

        winrt::Windows::Foundation::Collections::IVector<winrt::ProPractice::ExamAnswer> Answers();

        winrt::Windows::Foundation::IInspectable CustomDataContext() const;
        void CustomDataContext(winrt::Windows::Foundation::IInspectable const& value);

        bool IsAnswered();

    private:
        hstring _text;
        ExamQuestionType _type = ExamQuestionType::MultipleChoice;
        winrt::Windows::Foundation::Collections::IVector<winrt::ProPractice::ExamAnswer> _answers = nullptr;
        winrt::Windows::Foundation::IInspectable _customDataContext = nullptr;
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamQuestion : ExamQuestionT<ExamQuestion, implementation::ExamQuestion>
    {
    };
}
