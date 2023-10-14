#pragma once

#include "ExamControlHostPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamControlHostPage : ExamControlHostPageT<ExamControlHostPage>
    {
        ExamControlHostPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void OnControlAction(IInspectable const& sender, ExamControlAction action);

        Windows::Foundation::IAsyncAction LoadQuestions();

        [[nodiscard]] Windows::Foundation::IAsyncAction ShowErrorContentDialog(hstring const& title, hstring const& content) const;

    private:
        ProPractice::MainWindow _mainWindow = nullptr;
        ProPractice::ExamController _examController = nullptr;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct ExamControlHostPage : ExamControlHostPageT<ExamControlHostPage, implementation::ExamControlHostPage>
    {
    };
}
