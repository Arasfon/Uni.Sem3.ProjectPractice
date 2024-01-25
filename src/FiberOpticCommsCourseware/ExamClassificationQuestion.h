#pragma once

#include "ExamClassificationQuestion.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct ExamClassificationQuestion : ExamClassificationQuestionT<ExamClassificationQuestion>
    {
        ExamClassificationQuestion() = delete;

        ExamClassificationQuestion(int64_t id, hstring const& text);

        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IVector<winrt::FiberOpticCommsCourseware::ExamClassificationAnswer>, Answers, single_threaded_vector<winrt::FiberOpticCommsCourseware::ExamClassificationAnswer>());
        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IVector<hstring>, Categories, single_threaded_vector<hstring>());
        WINRT_READONLY_PROPERTY(int64_t, Id);
        WINRT_READONLY_PROPERTY(hstring, Text);
        WINRT_READONLY_PROPERTY(winrt::FiberOpticCommsCourseware::ExamQuestionType, Type, ExamQuestionType::Classification);

        [[nodiscard]] bool IsAnswered() const;
        [[nodiscard]] bool IsAnsweredCorrectly() const;
    };
}
namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct ExamClassificationQuestion : ExamClassificationQuestionT<ExamClassificationQuestion, implementation::ExamClassificationQuestion>
    {
    };
}
