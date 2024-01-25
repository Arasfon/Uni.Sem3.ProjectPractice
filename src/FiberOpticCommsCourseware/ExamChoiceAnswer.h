#pragma once

#include "ExamChoiceAnswer.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
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
namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct ExamChoiceAnswer : ExamChoiceAnswerT<ExamChoiceAnswer, implementation::ExamChoiceAnswer>
    {
    };
}
