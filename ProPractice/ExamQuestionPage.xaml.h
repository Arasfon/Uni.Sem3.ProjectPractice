#pragma once

#include "ExamQuestionPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamQuestionPage : ExamQuestionPageT<ExamQuestionPage>
    {
        ExamQuestionPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void ResetExamButtonClick(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ContinueExamButtonClick(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);

    private:
        ExamController _examController;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct ExamQuestionPage : ExamQuestionPageT<ExamQuestionPage, implementation::ExamQuestionPage>
    {
    };
}
