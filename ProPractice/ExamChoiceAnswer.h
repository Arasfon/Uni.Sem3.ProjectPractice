#pragma once

#include "ExamChoiceAnswer.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamChoiceAnswer : ExamChoiceAnswerT<ExamChoiceAnswer>
    {
        ExamChoiceAnswer() = delete;

        ExamChoiceAnswer(hstring const& text, bool correct);

        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(bool, Correct);
        WINRT_PROPERTY(bool, Chosen, false);
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamChoiceAnswer : ExamChoiceAnswerT<ExamChoiceAnswer, implementation::ExamChoiceAnswer>
    {
    };
}
