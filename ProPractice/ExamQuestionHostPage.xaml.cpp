#include "pch.h"
#include "ExamQuestionHostPage.xaml.h"
#if __has_include("ExamQuestionHostPage.g.cpp")
#include "ExamQuestionHostPage.g.cpp"
#endif

#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Microsoft::UI::Xaml::Media::Animation;

namespace winrt::ProPractice::implementation
{
    ExamQuestionHostPage::ExamQuestionHostPage()
    {
        InitializeComponent();
    }

    void ExamQuestionHostPage::OnNavigatedTo(NavigationEventArgs const& e)
    {
        _examController = unbox_value<ExamController>(e.Parameter());

        // ReSharper disable once CppExpressionWithoutSideEffects
        _examController.OnControlAction({ this, &ExamQuestionHostPage::OnControlAction });

        ExamProgressBar().Maximum(_examController.Questions().Size());
        UpdateProgressStatuses(0);

        const auto transitionInfo = SlideNavigationTransitionInfo();
        transitionInfo.Effect(SlideNavigationTransitionEffect::FromRight);
        // ReSharper disable once CppExpressionWithoutSideEffects
        ContentFrame().Navigate(xaml_typename<ExamQuestionPage>(), _examController, transitionInfo);
    }

    void ExamQuestionHostPage::OnControlAction(IInspectable const&, const ExamControlAction action)
    {
        if (action != ExamControlAction::Continue)
            return;

        UpdateProgressStatuses(_examController.CurrentQuestion() + 1);

        if (_examController.CurrentQuestion() + 1 == _examController.Questions().Size())
            return;

        _examController.CurrentQuestion(_examController.CurrentQuestion() + 1);

        if (_examController.CurrentQuestion() + 1 == _examController.Questions().Size())
            ContinueExamButton().Content(box_value(L"Завершить тестирование"));

        const auto transitionInfo = SlideNavigationTransitionInfo();
        transitionInfo.Effect(SlideNavigationTransitionEffect::FromRight);
        // ReSharper disable once CppExpressionWithoutSideEffects
        ContentFrame().Navigate(xaml_typename<ExamQuestionPage>(), _examController, transitionInfo);
    }

    void ExamQuestionHostPage::UpdateProgressStatuses(const unsigned int currentQuestion)
    {
        ExamProgressBar().Value(currentQuestion);

        std::wostringstream stringStream;
        stringStream << L"Выполнено " << currentQuestion << L"/" << _examController.Questions().Size();
        ExamProgressTextBlock().Text(stringStream.str());
    }

    void ExamQuestionHostPage::ResetExamButtonClick(IInspectable const&, RoutedEventArgs const&)
    {
        _examController.CallControl(ExamControlAction::Reset);
    }

    void ExamQuestionHostPage::ContinueExamButtonClick(IInspectable const&, RoutedEventArgs const&)
    {
        _examController.CallControl(ExamControlAction::Continue);
    }
}
