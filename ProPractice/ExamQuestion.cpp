#include "pch.h"
#include "ExamQuestion.h"
#if __has_include("ExamQuestion.g.cpp")
#include "ExamQuestion.g.cpp"
#endif

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

    Windows::Foundation::Collections::IVector<ExamAnswer> ExamQuestion::Answers()
    {
        return _answers;
    }
}
