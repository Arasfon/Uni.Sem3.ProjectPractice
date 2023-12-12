#include "pch.h"

#include "ExamResultsPage.xaml.h"
#if __has_include("ExamResultsPage.g.cpp")
#include "ExamResultsPage.g.cpp"
#endif

#include <format>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Xaml::Navigation;

namespace winrt::ProPractice::implementation
{
    ExamResultsPage::ExamResultsPage()
    {
        InitializeComponent();
    }

    void ExamResultsPage::OnNavigatedTo(NavigationEventArgs const& e)
    {
        _examController = unbox_value<ExamController>(e.Parameter());

        int correctCount = 0;

        for (unsigned int i = 0; i < _examController.Questions().Size(); i++)
        {
            auto question = _examController.Questions().GetAt(i);

            const bool isCorrect = question.IsAnsweredCorrectly();

            StackPanel sp;
            sp.Padding({ 16, 16, 16, 16 });

            TextBlock questionNumberTextBlock;
            questionNumberTextBlock.Text(L"Вопрос " + std::to_wstring(i + 1));
            questionNumberTextBlock.TextWrapping(TextWrapping::NoWrap);
            questionNumberTextBlock.Width(120);
            questionNumberTextBlock.Style(unbox_value<Microsoft::UI::Xaml::Style>(Application::Current().Resources().Lookup(box_value(L"SubtitleTextBlockStyle"))));

            sp.Children().Append(questionNumberTextBlock);

            TextBlock isCorrectTextBlock;

            if (isCorrect)
            {
                sp.Background(unbox_value<Brush>(Application::Current().Resources().Lookup(box_value(L"SystemFillColorSuccessBackgroundBrush"))));
                isCorrectTextBlock.Text(L"Верный ответ");
            }
            else
            {
                sp.Background(unbox_value<Brush>(Application::Current().Resources().Lookup(box_value(L"SystemFillColorCriticalBackgroundBrush"))));
                isCorrectTextBlock.Text(L"Неверный ответ");
            }

            sp.Children().Append(isCorrectTextBlock);
            ResultsGridView().Items().Append(sp);

            correctCount += isCorrect ? 1 : 0;
        }

        ResultTextBlock().Text(L"Результат: " + std::to_wstring(correctCount) + L"/" + std::to_wstring(_examController.Questions().Size()) + L" (" + std::format(L"{:.1f}", std::round(static_cast<double>(correctCount) / _examController.Questions().Size() * 100 * 10) / 10) + L"%)");
    }

    void ExamResultsPage::ResetExamButtonClick(IInspectable const&, RoutedEventArgs const&) const
    {
        _examController.CallControl(ExamControlAction::Reset);
    }
}
