#pragma once

#include "ExamAnswer.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamAnswer : ExamAnswerT<ExamAnswer>
    {
        ExamAnswer() = default;

        hstring Text();
        void Text(hstring const& value);

        bool IsCorrect();
        void IsCorrect(bool value);

        bool IsChosen();
        void IsChosen(bool value);

    private:
        hstring _text;
        bool _isCorrect;
        bool _isChosen;
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamAnswer : ExamAnswerT<ExamAnswer, implementation::ExamAnswer>
    {
    };
}
