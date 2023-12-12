#pragma once

#include "ExamOrderingAnswer.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamOrderingAnswer : ExamOrderingAnswerT<ExamOrderingAnswer>
    {
        ExamOrderingAnswer() = delete;

        ExamOrderingAnswer(hstring const& text, uint32_t correctOrder);

        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(uint32_t, CorrectOrder);
        WINRT_PROPERTY(uint32_t, Order);
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamOrderingAnswer : ExamOrderingAnswerT<ExamOrderingAnswer, implementation::ExamOrderingAnswer>
    {
    };
}
