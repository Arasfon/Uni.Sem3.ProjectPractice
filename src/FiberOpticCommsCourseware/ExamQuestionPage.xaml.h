#pragma once

#include "ExamQuestionPage.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct ExamQuestionPage : ExamQuestionPageT<ExamQuestionPage>
    {
        ExamQuestionPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

    private:
        ExamController _examController;
        bool _enterHandled = false;
        bool _validDropReceived = false;
    };
}

namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct ExamQuestionPage : ExamQuestionPageT<ExamQuestionPage, implementation::ExamQuestionPage>
    {
    };
}
