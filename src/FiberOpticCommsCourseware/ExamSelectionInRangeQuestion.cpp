#include "pch.h"

#include "ExamSelectionInRangeQuestion.h"
#if __has_include("ExamSelectionInRangeQuestion.g.cpp")
#include "ExamSelectionInRangeQuestion.g.cpp"
#endif

namespace winrt::FiberOpticCommsCourseware::implementation
{
    ExamSelectionInRangeQuestion::ExamSelectionInRangeQuestion(const int64_t id, hstring const& text, const double_t minimum, const double_t maximum, const double_t step, const double_t correctAnswer)
        : _Minimum { minimum }, _Maximum { maximum }, _Step { step }, _CorrectAnswer { correctAnswer }, _UserAnswer { minimum },
        _Id { id }, _Text { text }
    {
        
    }

    bool ExamSelectionInRangeQuestion::IsAnswered() const
    {
        return true;
    }

    bool ExamSelectionInRangeQuestion::IsAnsweredCorrectly() const
    {
        return UserAnswer() == CorrectAnswer();
    }
}
