#pragma once

#include "ExamChoiceQuestion.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamChoiceQuestion : ExamChoiceQuestionT<ExamChoiceQuestion>
    {
        ExamChoiceQuestion() = delete;

        ExamChoiceQuestion(int64_t id, hstring const& text, winrt::ProPractice::ExamQuestionType const& type);

        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IObservableVector<winrt::ProPractice::ExamChoiceAnswer>, Answers, single_threaded_observable_vector<winrt::ProPractice::ExamChoiceAnswer>());
        WINRT_READONLY_PROPERTY(int64_t, Id);
        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(winrt::ProPractice::ExamQuestionType, Type);

        [[nodiscard]] bool IsAnswered() const;
        [[nodiscard]] bool IsAnsweredCorrectly() const;
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamChoiceQuestion : ExamChoiceQuestionT<ExamChoiceQuestion, implementation::ExamChoiceQuestion>
    {
    };
}
