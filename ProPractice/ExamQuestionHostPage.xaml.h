#pragma once

#include "ExamQuestionHostPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamQuestionHostPage : ExamQuestionHostPageT<ExamQuestionHostPage>
    {
        ExamQuestionHostPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void OnControlAction(winrt::Windows::Foundation::IInspectable const& sender, const ExamControlAction action);

        void UpdateProgressStatuses(unsigned int currentQuestion);

        void ResetExamButtonClick(winrt::Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ContinueExamButtonClick(winrt::Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);

    private:
        ExamController _examController;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct ExamQuestionHostPage : ExamQuestionHostPageT<ExamQuestionHostPage, implementation::ExamQuestionHostPage>
    {
    };
}
