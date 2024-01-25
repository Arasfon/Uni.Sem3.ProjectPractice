#pragma once

#include "ExamOrderingQuestion.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct ExamOrderingQuestion : ExamOrderingQuestionT<ExamOrderingQuestion>
    {
        ExamOrderingQuestion() = delete;

        ExamOrderingQuestion(int64_t id, hstring const& text);

        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IObservableVector<winrt::FiberOpticCommsCourseware::ExamOrderingAnswer>, Answers, single_threaded_observable_vector<winrt::FiberOpticCommsCourseware::ExamOrderingAnswer>());
        WINRT_READONLY_PROPERTY(int64_t, Id);
        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(winrt::FiberOpticCommsCourseware::ExamQuestionType, Type, ExamQuestionType::Ordering);

        [[nodiscard]] bool IsAnswered() const;
        [[nodiscard]] bool IsAnsweredCorrectly() const;
    };
}
namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct ExamOrderingQuestion : ExamOrderingQuestionT<ExamOrderingQuestion, implementation::ExamOrderingQuestion>
    {
    };
}
