#include "pch.h"

#include "ExamChoiceQuestion.h"
#if __has_include("ExamChoiceQuestion.g.cpp")
#include "ExamChoiceQuestion.g.cpp"
#endif

namespace winrt::FiberOpticCommsCourseware::implementation
{
    ExamChoiceQuestion::ExamChoiceQuestion(const int64_t id, hstring const& text, winrt::FiberOpticCommsCourseware::ExamQuestionType const& type)
        : _Id { id }, _Text { text }, _Type { type }
    {
        
    }

    bool ExamChoiceQuestion::IsAnswered() const
    {
        return std::ranges::any_of(Answers(), [](ExamChoiceAnswer const& x)
        {
            return x.Chosen();
        });
    }

    bool ExamChoiceQuestion::IsAnsweredCorrectly() const
    {
        return std::ranges::all_of(Answers(), [](ExamChoiceAnswer const& x)
        {
            return x.Chosen() == x.Correct();
        });
    }
}
