#include "pch.h"

#include "ExamOrderingQuestion.h"
#if __has_include("ExamOrderingQuestion.g.cpp")
#include "ExamOrderingQuestion.g.cpp"
#endif

namespace winrt::ProPractice::implementation
{
    ExamOrderingQuestion::ExamOrderingQuestion(const int64_t id, hstring const& text)
        : _Id { id }, _Text { text }
    {
        
    }

    bool ExamOrderingQuestion::IsAnswered() const
    {
        return true;
    }

    bool ExamOrderingQuestion::IsAnsweredCorrectly() const
    {
        for (unsigned int i = 0; i < Answers().Size(); i++)
        {
            if (Answers().GetAt(i).CorrectOrder() != i)
                return false;
        }

        return true;
    }
}
