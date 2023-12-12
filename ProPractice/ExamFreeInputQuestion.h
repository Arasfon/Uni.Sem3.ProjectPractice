#pragma once

#include "ExamFreeInputQuestion.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamFreeInputQuestion : ExamFreeInputQuestionT<ExamFreeInputQuestion>
    {
        ExamFreeInputQuestion() = delete;

        ExamFreeInputQuestion(int64_t id, hstring const& text);

        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IVector<hstring>, CorrectAnswers, single_threaded_vector<hstring>());
        WINRT_PROPERTY(hstring, UserAnswer, L"");

        WINRT_READONLY_PROPERTY(int64_t, Id);
        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(winrt::ProPractice::ExamQuestionType, Type, ExamQuestionType::FreeInput);

        [[nodiscard]] bool IsAnswered() const;
        [[nodiscard]] bool IsAnsweredCorrectly() const;
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamFreeInputQuestion : ExamFreeInputQuestionT<ExamFreeInputQuestion, implementation::ExamFreeInputQuestion>
    {
    };
}
