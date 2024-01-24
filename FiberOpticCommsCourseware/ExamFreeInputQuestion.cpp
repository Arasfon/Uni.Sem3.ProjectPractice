#include "pch.h"

#include "ExamFreeInputQuestion.h"
#if __has_include("ExamFreeInputQuestion.g.cpp")
#include "ExamFreeInputQuestion.g.cpp"
#endif

namespace winrt::FiberOpticCommsCourseware::implementation
{
    ExamFreeInputQuestion::ExamFreeInputQuestion(const int64_t id, hstring const& text)
        : _Id { id }, _Text { text }
    {
        
    }
    
    bool ExamFreeInputQuestion::IsAnswered() const
    {
        return !UserAnswer().empty();
    }

    bool ExamFreeInputQuestion::IsAnsweredCorrectly() const
    {
        return std::ranges::any_of(CorrectAnswers(), [this](hstring const& correctAnswer)
        {
            // ReSharper disable once CppZeroConstantCanBeReplacedWithNullptr
            return CSTR_EQUAL == CompareStringEx(LOCALE_NAME_USER_DEFAULT, LINGUISTIC_IGNORECASE, UserAnswer().c_str(), -1, correctAnswer.c_str(), -1, nullptr, nullptr, 0);
        });
    }
}
