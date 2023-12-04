#pragma once

#include "ExamAnswer.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamAnswer : ExamAnswerT<ExamAnswer>
    {
        ExamAnswer() = default;

        WINRT_PROPERTY(hstring, Text);
        WINRT_PROPERTY(bool, IsCorrect, false);
        WINRT_PROPERTY(bool, IsChosen, false);
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamAnswer : ExamAnswerT<ExamAnswer, implementation::ExamAnswer>
    {
    };
}
