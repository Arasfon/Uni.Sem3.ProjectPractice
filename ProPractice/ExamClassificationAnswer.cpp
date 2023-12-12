#include "pch.h"

#include "ExamClassificationAnswer.h"
#if __has_include("ExamClassificationAnswer.g.cpp")
#include "ExamClassificationAnswer.g.cpp"
#endif

namespace winrt::ProPractice::implementation
{
    ExamClassificationAnswer::ExamClassificationAnswer(hstring const& text, const int32_t correctCategoryId)
        : _Text { text }, _CorrectCategoryId { correctCategoryId }
    {
        
    }
}
