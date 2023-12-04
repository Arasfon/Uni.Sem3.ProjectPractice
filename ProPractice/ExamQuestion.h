#pragma once

#include "ExamQuestion.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamQuestion : ExamQuestionT<ExamQuestion>
    {
        ExamQuestion() = default;

        WINRT_PROPERTY(hstring, Text);
        WINRT_PROPERTY(winrt::ProPractice::ExamQuestionType, Type, ExamQuestionType::MultipleChoice);
        WINRT_PROPERTY(winrt::Windows::Foundation::IInspectable, CustomDataContext, nullptr);

        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IVector<winrt::ProPractice::ExamAnswer>, Answers, winrt::single_threaded_vector<ExamAnswer>());

        bool IsAnswered() const;
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamQuestion : ExamQuestionT<ExamQuestion, implementation::ExamQuestion>
    {
    };
}
