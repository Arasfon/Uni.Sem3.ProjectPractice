#include "pch.h"
#include "ExamQuestionHostPage.xaml.h"
#if __has_include("ExamQuestionHostPage.g.cpp")
#include "ExamQuestionHostPage.g.cpp"
#endif

#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::ProPractice::implementation
{
    ExamQuestionHostPage::ExamQuestionHostPage()
    {
        InitializeComponent();
    }

    void ExamQuestionHostPage::OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {
        _examController = unbox_value<ExamController>(e.Parameter());

        // ReSharper disable once CppExpressionWithoutSideEffects
        _examController.OnControlAction({ this, &ExamQuestionHostPage::OnControlAction });

        const auto transitionInfo = winrt::Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo();
        transitionInfo.Effect(Media::Animation::SlideNavigationTransitionEffect::FromRight);
        // ReSharper disable once CppExpressionWithoutSideEffects
        ContentFrame().Navigate(xaml_typename<ExamQuestionPage>(), _examController, transitionInfo);
    }

    void ExamQuestionHostPage::OnControlAction(IInspectable const&, const ExamControlAction action)
    {
        if (action != ExamControlAction::Continue || _examController.CurrentQuestion() + 1 == _examController.Questions().Size())
            return;

        _examController.CurrentQuestion(_examController.CurrentQuestion() + 1);

        if (_examController.CurrentQuestion() + 1 == _examController.Questions().Size())
            ContinueExamButton().Content(box_value(L"Завершить тестирование"));

        const auto transitionInfo = winrt::Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo();
        transitionInfo.Effect(Media::Animation::SlideNavigationTransitionEffect::FromRight);
        // ReSharper disable once CppExpressionWithoutSideEffects
        ContentFrame().Navigate(xaml_typename<ExamQuestionPage>(), _examController, transitionInfo);
    }

    void ExamQuestionHostPage::ResetExamButtonClick(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        _examController.CallControl(ExamControlAction::Reset);
    }

    void ExamQuestionHostPage::ContinueExamButtonClick(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        _examController.CallControl(ExamControlAction::Continue);
    }
}
