#pragma once

#include "ExamResultsPage.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct ExamResultsPage : ExamResultsPageT<ExamResultsPage>
    {
        ExamResultsPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void ResetExamButtonClick(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e) const;

    private:
        ExamController _examController;
    };
}

namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct ExamResultsPage : ExamResultsPageT<ExamResultsPage, implementation::ExamResultsPage>
    {
    };
}
