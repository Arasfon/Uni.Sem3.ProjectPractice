#include "pch.h"

#include "ExamClassificationQuestion.h"
#if __has_include("ExamClassificationQuestion.g.cpp")
#include "ExamClassificationQuestion.g.cpp"
#endif

namespace winrt::ProPractice::implementation
{
    ExamClassificationQuestion::ExamClassificationQuestion(const int64_t id, hstring const& text)
        : _Id{id}, _Text{text}
    {
    }

    bool ExamClassificationQuestion::IsAnswered() const
    {
        return std::ranges::any_of(Answers(), [](ExamClassificationAnswer const& x)
        {
            return x.UserCategoryId() != -1;
        });
    }

    bool ExamClassificationQuestion::IsAnsweredCorrectly() const
    {
        return std::ranges::all_of(Answers(), [](ExamClassificationAnswer const& x)
        {
            return std::cmp_equal(x.UserCategoryId(), x.CorrectCategoryId());
        });
    }
}
