#include "pch.h"
#include "ExamQuestion.h"
#if __has_include("ExamQuestion.g.cpp")
#include "ExamQuestion.g.cpp"
#endif

using namespace winrt;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation;

namespace winrt::ProPractice::implementation
{
    bool ExamQuestion::IsAnswered() const
    {
        if (Type() == ExamQuestionType::FreeInput)
            return CustomDataContext() != nullptr;

        bool isAnswered = false;

        for (auto answer : Answers())
        {
            if (answer.IsChosen())
            {
                isAnswered = true;
                break;
            }
        }

        return isAnswered;
    }
}
