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

        _controlActionEventToken = _examController.OnControlAction({ this, &ExamQuestionHostPage::OnControlAction });

        ExamProgressBar().Maximum(_examController.Questions().Size());
        UpdateProgressStatuses(0);
        ContinueBackwardExamButton().IsEnabled(false);

        _remainingTime = std::chrono::minutes { _examController.Questions().Size() };
        _remainingTime -= std::chrono::seconds { 1 }; // Because timer does not fire Tick event when started
        UpdateRemainingTime();
        _timer = DispatcherTimer();
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
        if (action == ExamControlAction::End || action == ExamControlAction::Reset)
        {
            _examController.OnControlAction(_controlActionEventToken);
            _timer.Stop();

            return;
        }

        if (action == ExamControlAction::QuestionSelect)
        {
            _examController.CurrentQuestion(_examController.Questions().Size() - 1);
            ContinueForwardExamButton().Content(box_value(L"Завершить тестирование"));
            ContinueBackwardExamButton().Visibility(Visibility::Collapsed);
            QuestionSelectExamButton().Visibility(Visibility::Collapsed);

            // ReSharper disable once CppExpressionWithoutSideEffects
            ContentFrame().Navigate(xaml_typename<ExamQuestionSelectPage>(), _examController);

            return;
        }

        if (action == ExamControlAction::JumpToCurrentQuestion)
        {
            if (_examController.CurrentQuestion() == 0)
                ContinueBackwardExamButton().IsEnabled(false);
            else
                ContinueBackwardExamButton().IsEnabled(true);

            if (_examController.CurrentQuestion() + 1 == _examController.Questions().Size())
                ContinueForwardExamButton().Content(box_value(L"Завершить тестирование"));
            else
                ContinueForwardExamButton().Content(box_value(L"Следующий вопрос"));

            ContinueBackwardExamButton().Visibility(Visibility::Visible);
            QuestionSelectExamButton().Visibility(Visibility::Visible);

            UpdateProgressStatuses(_examController.CurrentQuestion());

            // ReSharper disable once CppExpressionWithoutSideEffects
            ContentFrame().Navigate(xaml_typename<ExamQuestionPage>(), _examController);

            return;
        }

        if (action != ExamControlAction::ContinueForward && action != ExamControlAction::ContinueBackward)
            return;

        const auto transitionInfo = SlideNavigationTransitionInfo();

        if (action == ExamControlAction::ContinueForward)
        {
            _examController.CurrentQuestion(_examController.CurrentQuestion() + 1);
            transitionInfo.Effect(SlideNavigationTransitionEffect::FromRight);
        }
        else
        {
            _examController.CurrentQuestion(_examController.CurrentQuestion() - 1);
            transitionInfo.Effect(SlideNavigationTransitionEffect::FromLeft);
        }
        
        UpdateProgressStatuses(_examController.CurrentQuestion());

        if (_examController.CurrentQuestion() == 0)
            ContinueBackwardExamButton().IsEnabled(false);
        else
            ContinueBackwardExamButton().IsEnabled(true);

        if (_examController.CurrentQuestion() + 1 == _examController.Questions().Size())
            ContinueForwardExamButton().Content(box_value(L"Завершить тестирование"));
        else
            ContinueForwardExamButton().Content(box_value(L"Следующий вопрос"));

        // ReSharper disable once CppExpressionWithoutSideEffects
        ContentFrame().Navigate(xaml_typename<ExamQuestionPage>(), _examController, transitionInfo);
    }

    void ExamQuestionHostPage::UpdateProgressStatuses(const unsigned int currentQuestion)
    {
        ExamProgressBar().Value(currentQuestion);

        std::wostringstream stringStream;
        stringStream << L"Вопрос " << currentQuestion + 1 << L"/" << _examController.Questions().Size();
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
            _examController.CallControl(ExamControlAction::End);
            return;
        }

        _remainingTime -= std::chrono::seconds{ 1 };
        UpdateRemainingTime();
    }

    void ExamQuestionHostPage::ResetExamButtonClick(IInspectable const&, RoutedEventArgs const&) const
    {
        _examController.CallControl(ExamControlAction::Reset);
    }

    void ExamQuestionHostPage::ContinueForwardExamButtonClick(IInspectable const&, RoutedEventArgs const&)
    {
        if (_examController.CurrentQuestion() + 1 != _examController.Questions().Size())
        {
            _examController.CallControl(ExamControlAction::ContinueForward);
            return;
        }

        if (unbox_value_or<ExamQuestionSelectPage>(ContentFrame().Content(), nullptr) != nullptr)
            {
                _examController.CallControl(ExamControlAction::End);
                return;
            }

        bool hasUnansweredQuestions = false;

        for (auto question : _examController.Questions())
        {
            if (!question.IsAnswered())
            {
                hasUnansweredQuestions = true;
                break;
            }
        }

        if (hasUnansweredQuestions)
            _examController.CallControl(ExamControlAction::QuestionSelect);
        else
            _examController.CallControl(ExamControlAction::End);
    }

    void ExamQuestionHostPage::ContinueBackwardExamButtonClick(IInspectable const&, RoutedEventArgs const&) const
    {
        _examController.CallControl(ExamControlAction::ContinueBackward);
    }

    void ExamQuestionHostPage::QuestionSelectExamButtonClick(IInspectable const&, RoutedEventArgs const&) const
    {
        _examController.CallControl(ExamControlAction::QuestionSelect);
    }
}
