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
using namespace Windows::Foundation;

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

        _remainingTime = std::chrono::minutes { _examController.Questions().Size() };
        _remainingTime -= std::chrono::seconds { 1 }; // Because timer does not fire Tick event when started
        UpdateRemainingTime();
        _timer.Interval(TimeSpan { std::chrono::seconds { 1 } });
        // ReSharper disable once CppExpressionWithoutSideEffects
        _timer.Tick({ this, &ExamQuestionHostPage::TimerTick });
        _timer.Start();

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
        {
            _timer.Stop();
            return;
        }

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

    void ExamQuestionHostPage::UpdateRemainingTime()
    {
        std::wostringstream stringStream;
        stringStream << L"Осталось " <<
            std::format(L"{:02}", std::chrono::duration_cast<std::chrono::minutes>(_remainingTime).count()) <<
            L":" <<
            std::format(L"{:02}", (_remainingTime % 60).count());
        RemainingTimeTextBlock().Text(stringStream.str());
    }

    void ExamQuestionHostPage::TimerTick(IInspectable const&, IInspectable const&)
    {
        if (_remainingTime.count() == 0)
        {
            _timer.Stop();
            _examController.CurrentQuestion(_examController.Questions().Size() - 1);
            _examController.CallControl(ExamControlAction::Continue);
            return;
        }

        _remainingTime -= std::chrono::seconds{ 1 };
        UpdateRemainingTime();
    }

    void ExamQuestionHostPage::ResetExamButtonClick(IInspectable const&, RoutedEventArgs const&) const
    {
        _timer.Stop();
        _examController.CallControl(ExamControlAction::Reset);
    }

    void ExamQuestionHostPage::ContinueExamButtonClick(IInspectable const&, RoutedEventArgs const&) const
    {
        _examController.CallControl(ExamControlAction::Continue);
    }
}
