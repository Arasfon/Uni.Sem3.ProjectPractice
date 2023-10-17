#include "pch.h"
#include "ExamAnswer.h"
#if __has_include("ExamAnswer.g.cpp")
#include "ExamAnswer.g.cpp"
#endif

namespace winrt::ProPractice::implementation
{
    hstring ExamAnswer::Text()
    {
        return _text;
    }

    void ExamAnswer::Text(hstring const& value)
    {
        _text = value;
    }

    bool ExamAnswer::IsCorrect() const
    {
        return _isCorrect;
    }

    void ExamAnswer::IsCorrect(bool value)
    {
        _isCorrect = value;
    }

    bool ExamAnswer::IsChosen() const
    {
        return _isChosen;
    }

    void ExamAnswer::IsChosen(bool value)
    {
        _isChosen = value;
    }
}
