#pragma once

#include "ExamStartPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamStartPage : ExamStartPageT<ExamStartPage>
    {
        ExamStartPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void StartButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);

    private:
        ProPractice::ExamController _examController;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct ExamStartPage : ExamStartPageT<ExamStartPage, implementation::ExamStartPage>
    {
    };
}
