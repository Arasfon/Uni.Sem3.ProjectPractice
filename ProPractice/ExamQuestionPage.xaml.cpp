#include "pch.h"
#include "ExamQuestionPage.xaml.h"
#if __has_include("ExamQuestionPage.g.cpp")
#include "ExamQuestionPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::ProPractice::implementation
{
    ExamQuestionPage::ExamQuestionPage()
    {
        InitializeComponent();
    }

    void ExamQuestionPage::OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {
        _examController = unbox_value<ExamController>(e.Parameter());

        const auto question = _examController.Questions().GetAt(_examController.CurrentQuestion());

        QuestionTextTextBlock().Text(question.Text());

        switch (question.Type())
        {
            case ExamQuestionType::MultipleChoice:
            {
                AnswerTypeTextBlock().Text(L"Выберите несколько вариантов ответа:");

                for (unsigned int i = 0; i < question.Answers().Size(); i++)
                {
                    auto answer = question.Answers().GetAt(i);

                    const CheckBox answerCheckBox;

                    answerCheckBox.Content(box_value(answer.Text()));
                    answerCheckBox.Tag(box_value(i));
                    answerCheckBox.Click([this](IInspectable const& sender, RoutedEventArgs const&)
                        {
                            const CheckBox cb = unbox_value<CheckBox>(sender);
                            const int id = unbox_value<unsigned int>(cb.Tag());
                            _examController.Questions().GetAt(_examController.CurrentQuestion()).Answers().GetAt(id).IsChosen(cb.IsChecked().Value());
                        });

                    ContentStackPanel().Children().Append(answerCheckBox);
                }

                break;
            }
            case ExamQuestionType::SingleChoice:
            {
                AnswerTypeTextBlock().Text(L"Выберите один из вариантов ответа:");

                const RadioButtons radioButtons;

                for (unsigned int i = 0; i < question.Answers().Size(); i++)
                {
                    auto answer = question.Answers().GetAt(i);

                    const RadioButton answerRadioButton;

                    answerRadioButton.Content(box_value(answer.Text()));
                    answerRadioButton.Tag(box_value(i));
                    
                    radioButtons.Items().Append(answerRadioButton);
                }

                radioButtons.SelectionChanged([this](IInspectable const&, SelectionChangedEventArgs const& e)
                    {
                        const auto deselectedItem = unbox_value<RadioButton>(e.RemovedItems().GetAt(0));
                        const auto selectedItem = unbox_value<RadioButton>(e.AddedItems().GetAt(0));

                        if (deselectedItem != nullptr)
                            _examController.Questions().GetAt(_examController.CurrentQuestion()).Answers().GetAt(unbox_value<unsigned int>(deselectedItem.Tag())).IsChosen(false);
                        if (selectedItem != nullptr)
                            _examController.Questions().GetAt(_examController.CurrentQuestion()).Answers().GetAt(unbox_value<unsigned int>(selectedItem.Tag())).IsChosen(true);
                    });

                ContentStackPanel().Children().Append(radioButtons);

                break;
            }
            case ExamQuestionType::FreeInput:
            {
                AnswerTypeTextBlock().Text(L"Введите свой ответ:");

                const TextBox textBox;

                textBox.Width(200);
                textBox.Margin({ 0, 5, 0, 0 });
                textBox.PlaceholderText(L"Ответ");

                // TODO: Consider LostFocus or similar (for performance reasons)
                textBox.TextChanged([this](IInspectable const& sender, TextChangedEventArgs const&)
                    {
                        const auto tb = unbox_value<TextBox>(sender);

                        for (auto answer : _examController.Questions().GetAt(_examController.CurrentQuestion()).Answers())
                        {
                            if (answer.Text() == tb.Text())
                                answer.IsChosen(true);
                            else
                                answer.IsChosen(false);
                        }
                    });

                ContentStackPanel().Children().Append(textBox);

                break;
            }
        }
    }
}
