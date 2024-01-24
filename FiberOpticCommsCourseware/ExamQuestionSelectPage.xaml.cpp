#include "pch.h"
#include "ExamQuestionSelectPage.xaml.h"
#if __has_include("ExamQuestionSelectPage.g.cpp")
#include "ExamQuestionSelectPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Xaml::Navigation;

namespace winrt::FiberOpticCommsCourseware::implementation
{
    ExamQuestionSelectPage::ExamQuestionSelectPage()
    {
        InitializeComponent();
    }

    void ExamQuestionSelectPage::OnNavigatedTo(NavigationEventArgs const& e)
    {
        _examController = unbox_value<ExamController>(e.Parameter());

        for (unsigned int i = 0; i < _examController.Questions().Size(); i++)
        {
            auto question = _examController.Questions().GetAt(i);

            StackPanel sp;
            sp.Padding({ 16, 16, 16, 16 });
            sp.Tag(box_value(i));

            TextBlock questionNumberTextBlock;
            questionNumberTextBlock.Text(L"Вопрос " + std::to_wstring(i + 1));
            questionNumberTextBlock.TextWrapping(TextWrapping::NoWrap);
            questionNumberTextBlock.Width(120);
            questionNumberTextBlock.Style(unbox_value<Microsoft::UI::Xaml::Style>(Application::Current().Resources().Lookup(box_value(L"SubtitleTextBlockStyle"))));

            sp.Children().Append(questionNumberTextBlock);

            TextBlock isCorrectTextBlock;

            if (question.IsAnswered())
            {
                sp.Background(unbox_value<Brush>(Application::Current().Resources().Lookup(box_value(L"CardBackgroundFillColorDefaultBrush"))));
                isCorrectTextBlock.Text(L"Отвечено");
            }
            else
            {
                sp.Background(unbox_value<Brush>(Application::Current().Resources().Lookup(box_value(L"SystemFillColorCautionBackgroundBrush"))));
                isCorrectTextBlock.Text(L"Не отвечено");
            }

            sp.Children().Append(isCorrectTextBlock);
            QuestionsGridView().Items().Append(sp);
        }
    }

    void ExamQuestionSelectPage::QuestionsGridViewItemClick(IInspectable const&, ItemClickEventArgs const& e) const
    {
        _examController.CurrentQuestion(unbox_value<unsigned int>(unbox_value<FrameworkElement>(e.ClickedItem()).Tag()));
        _examController.CallControl(ExamControlAction::JumpToCurrentQuestion);
    }
}
