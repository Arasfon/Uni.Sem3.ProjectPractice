#pragma once

#include "ExamQuestionSelectPage.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct ExamQuestionSelectPage : ExamQuestionSelectPageT<ExamQuestionSelectPage>
    {
        ExamQuestionSelectPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void QuestionsGridViewItemClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::ItemClickEventArgs const& e) const;

    private:
        ExamController _examController;
    };
}

namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct ExamQuestionSelectPage : ExamQuestionSelectPageT<ExamQuestionSelectPage, implementation::ExamQuestionSelectPage>
    {
    };
}
