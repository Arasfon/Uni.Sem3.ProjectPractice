#pragma once

#include "ExamClassificationQuestion.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamClassificationQuestion : ExamClassificationQuestionT<ExamClassificationQuestion>
    {
        ExamClassificationQuestion() = delete;

        ExamClassificationQuestion(int64_t id, hstring const& text);

        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IVector<winrt::ProPractice::ExamClassificationAnswer>, Answers, single_threaded_vector<winrt::ProPractice::ExamClassificationAnswer>());
        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IVector<hstring>, Categories, single_threaded_vector<hstring>());
        WINRT_READONLY_PROPERTY(int64_t, Id);
        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(winrt::ProPractice::ExamQuestionType, Type, ExamQuestionType::Classification);

        [[nodiscard]] bool IsAnswered() const;
        [[nodiscard]] bool IsAnsweredCorrectly() const;
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamClassificationQuestion : ExamClassificationQuestionT<ExamClassificationQuestion, implementation::ExamClassificationQuestion>
    {
    };
}
