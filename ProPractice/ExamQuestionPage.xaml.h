#pragma once

#include "ExamQuestionPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamQuestionPage : ExamQuestionPageT<ExamQuestionPage>
    {
        ExamQuestionPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

    private:
        ExamController _examController;
        bool _enterHandled = false;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct ExamQuestionPage : ExamQuestionPageT<ExamQuestionPage, implementation::ExamQuestionPage>
    {
    };
}
