#include "pch.h"

#include "ExamOrderingAnswer.h"
#if __has_include("ExamOrderingAnswer.g.cpp")
#include "ExamOrderingAnswer.g.cpp"
#endif

namespace winrt::ProPractice::implementation
{
    ExamOrderingAnswer::ExamOrderingAnswer(hstring const& text, const uint32_t correctOrder)
        : _Text { text }, _CorrectOrder { correctOrder }
    {
        
    }
}
