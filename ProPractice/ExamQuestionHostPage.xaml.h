#pragma once

#include "ExamQuestionHostPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamQuestionHostPage : ExamQuestionHostPageT<ExamQuestionHostPage>
    {
        ExamQuestionHostPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void OnControlAction(winrt::Windows::Foundation::IInspectable const& sender, ExamControlAction action);

        void UpdateProgressStatuses(unsigned int currentQuestion);
        void UpdateRemainingTime();

        void TimerTick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);

        void ResetExamButtonClick(winrt::Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e) const;
        void ContinueExamButtonClick(winrt::Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e) const;

    private:
        ExamController _examController;
        winrt::Microsoft::UI::Xaml::DispatcherTimer _timer;
        std::chrono::seconds _remainingTime = std::chrono::seconds{0};
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct ExamQuestionHostPage : ExamQuestionHostPageT<ExamQuestionHostPage, implementation::ExamQuestionHostPage>
    {
    };
}
