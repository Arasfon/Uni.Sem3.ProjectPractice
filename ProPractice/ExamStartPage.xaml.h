#pragma once

#include "ExamStartPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamStartPage : ExamStartPageT<ExamStartPage>
    {
        ExamStartPage();

        Windows::Foundation::IAsyncAction OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        Windows::Foundation::IAsyncOperation<int64_t> GetQuestionCount() const;

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
