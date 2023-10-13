#pragma once

#include "ExamResultsPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamResultsPage : ExamResultsPageT<ExamResultsPage>
    {
        ExamResultsPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void ResetExamButtonClick(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);

    private:
        ExamController _examController;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct ExamResultsPage : ExamResultsPageT<ExamResultsPage, implementation::ExamResultsPage>
    {
    };
}
