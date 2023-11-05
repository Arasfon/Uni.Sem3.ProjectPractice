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
    ExamQuestion::ExamQuestion()
    {
        _answers = single_threaded_vector<ExamAnswer>();
    }

    hstring ExamQuestion::Text()
    {
        return _text;
    }

    void ExamQuestion::Text(hstring const& value)
    {
        _text = value;
    }

    ExamQuestionType ExamQuestion::Type() const
    {
        return _type;
    }

    void ExamQuestion::Type(ExamQuestionType const& value)
    {
        _type = value;
    }

    IVector<ExamAnswer> ExamQuestion::Answers()
    {
        return _answers;
    }

    IInspectable ExamQuestion::CustomDataContext() const
    {
        return _customDataContext;
    }

    void ExamQuestion::CustomDataContext(IInspectable const& value)
    {
        _customDataContext = value;
    }

    bool ExamQuestion::IsAnswered()
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
