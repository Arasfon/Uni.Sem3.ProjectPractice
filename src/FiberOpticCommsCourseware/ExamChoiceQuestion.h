#pragma once

#include "ExamChoiceQuestion.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct ExamChoiceQuestion : ExamChoiceQuestionT<ExamChoiceQuestion>
    {
        ExamChoiceQuestion() = delete;

        ExamChoiceQuestion(int64_t id, hstring const& text, winrt::FiberOpticCommsCourseware::ExamQuestionType const& type);

        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IObservableVector<winrt::FiberOpticCommsCourseware::ExamChoiceAnswer>, Answers, single_threaded_observable_vector<winrt::FiberOpticCommsCourseware::ExamChoiceAnswer>());
        WINRT_READONLY_PROPERTY(int64_t, Id);
        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(winrt::FiberOpticCommsCourseware::ExamQuestionType, Type);

        [[nodiscard]] bool IsAnswered() const;
        [[nodiscard]] bool IsAnsweredCorrectly() const;
    };
}
namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct ExamChoiceQuestion : ExamChoiceQuestionT<ExamChoiceQuestion, implementation::ExamChoiceQuestion>
    {
    };
}
