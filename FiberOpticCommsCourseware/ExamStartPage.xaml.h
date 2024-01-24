#pragma once

#include "ExamStartPage.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct ExamStartPage : ExamStartPageT<ExamStartPage>
    {
        ExamStartPage();

        Windows::Foundation::IAsyncAction OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        [[nodiscard]] Windows::Foundation::IAsyncOperation<int64_t> GetQuestionCount() const;

        void StartButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e) const;

    private:
        FiberOpticCommsCourseware::ExamController _examController;
    };
}

namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct ExamStartPage : ExamStartPageT<ExamStartPage, implementation::ExamStartPage>
    {
    };
}
