#pragma once

#include "ExamClassificationAnswer.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamClassificationAnswer : ExamClassificationAnswerT<ExamClassificationAnswer>
    {
        ExamClassificationAnswer() = delete;

        ExamClassificationAnswer(hstring const& text, int32_t correctCategoryId);

        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(int32_t, CorrectCategoryId);
        WINRT_PROPERTY(int32_t, UserCategoryId, -1);
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamClassificationAnswer : ExamClassificationAnswerT<ExamClassificationAnswer, implementation::ExamClassificationAnswer>
    {
    };
}
