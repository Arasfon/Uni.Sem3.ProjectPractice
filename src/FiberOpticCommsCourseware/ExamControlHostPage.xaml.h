#pragma once

#include "ExamControlHostPage.g.h"

#include <random>

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct ExamControlHostPage : ExamControlHostPageT<ExamControlHostPage>
    {
        ExamControlHostPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void OnControlAction(IInspectable const& sender, ExamControlAction action);

        Windows::Foundation::IAsyncAction LoadQuestions() const;

    private:
        FiberOpticCommsCourseware::MainWindow _mainWindow = nullptr;
        FiberOpticCommsCourseware::ExamController _examController = nullptr;
        inline static std::random_device _randomDevice;
    };
}

namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct ExamControlHostPage : ExamControlHostPageT<ExamControlHostPage, implementation::ExamControlHostPage>
    {
    };
}
