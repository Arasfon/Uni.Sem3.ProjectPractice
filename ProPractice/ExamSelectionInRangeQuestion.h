#pragma once

#include "ExamSelectionInRangeQuestion.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamSelectionInRangeQuestion : ExamSelectionInRangeQuestionT<ExamSelectionInRangeQuestion>
    {
        ExamSelectionInRangeQuestion() = delete;

        ExamSelectionInRangeQuestion(int64_t id, hstring const& text, double_t minimum, double_t maximum, double_t step, double_t correctAnswer);

        WINRT_READONLY_PROPERTY(double_t, Minimum);
        WINRT_READONLY_PROPERTY(double_t, Maximum);
        WINRT_READONLY_PROPERTY(double_t, Step);
        WINRT_READONLY_PROPERTY(double_t, CorrectAnswer);
        WINRT_PROPERTY(double_t, UserAnswer);

        WINRT_READONLY_PROPERTY(int64_t, Id);
        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(winrt::ProPractice::ExamQuestionType, Type, ExamQuestionType::SelectionInRange);

        [[nodiscard]] bool IsAnswered() const;
        [[nodiscard]] bool IsAnsweredCorrectly() const;
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamSelectionInRangeQuestion : ExamSelectionInRangeQuestionT<ExamSelectionInRangeQuestion, implementation::ExamSelectionInRangeQuestion>
    {
    };
}
