#include "pch.h"

#include "ExamChoiceAnswer.h"
#if __has_include("ExamChoiceAnswer.g.cpp")
#include "ExamChoiceAnswer.g.cpp"
#endif

namespace winrt::FiberOpticCommsCourseware::implementation
{
    ExamChoiceAnswer::ExamChoiceAnswer(hstring const& text, const bool correct)
        : _Text { text }, _Correct { correct }
    {
        
    }
}
