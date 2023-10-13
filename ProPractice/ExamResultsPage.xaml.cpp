#include "pch.h"
#include "ExamResultsPage.xaml.h"
#if __has_include("ExamResultsPage.g.cpp")
#include "ExamResultsPage.g.cpp"
#endif

#include <sstream>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::ProPractice::implementation
{
    ExamResultsPage::ExamResultsPage()
    {
        InitializeComponent();
    }

    void ExamResultsPage::OnNavigatedTo(Navigation::NavigationEventArgs const& e)
    {
        _examController = unbox_value<ExamController>(e.Parameter());

        int correctCount = 0;
        //std::wostringstream resultsText;

        for (int i = 0; i < _examController.Questions().Size(); i++)
        {
            auto question = _examController.Questions().GetAt(i);

            bool isCorrect = true;

            for (auto answer : question.Answers())
            {
                if (answer.IsCorrect() != answer.IsChosen())
                {
                    isCorrect = false;
                    break;
                }
            }

            //resultsText << L"Ответ " << i + 1 << L": " << isCorrect ? L"верный" : L"неверный" << std::endl;
            correctCount += isCorrect ? 1 : 0;
        }

        ResultTextBlock().Text(L"Результат: " + std::to_wstring(correctCount) + L"/" + std::to_wstring(_examController.Questions().Size()));
    }

    void ExamResultsPage::ResetExamButtonClick(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        _examController.CallControl(ExamControlAction::Reset);
    }
}
