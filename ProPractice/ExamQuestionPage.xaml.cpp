#include "pch.h"
#include "ExamQuestionPage.xaml.h"
#if __has_include("ExamQuestionPage.g.cpp")
#include "ExamQuestionPage.g.cpp"
#endif

#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <sstream>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Input;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Windows::System;

namespace winrt::ProPractice::implementation
{
    ExamQuestionPage::ExamQuestionPage()
    {
        InitializeComponent();
    }

    void ExamQuestionPage::OnNavigatedTo(NavigationEventArgs const& e)
    {
        _examController = unbox_value<ExamController>(e.Parameter());

        const auto question = _examController.Questions().GetAt(_examController.CurrentQuestion());

        QuestionTextTextBlock().Text(question.Text());

        // Debug ↓
        std::wostringstream debugExamAnswersString;
        debugExamAnswersString << L"Debug: Правильный ответ: ";
        auto debugExamAnswersStringItemSeparator = L"";
        // Debug ↑

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
                    answerCheckBox.IsChecked(answer.IsChosen());
                    answerCheckBox.Click([this](IInspectable const& sender, RoutedEventArgs const&)
                        {
                            const CheckBox cb = unbox_value<CheckBox>(sender);
                            const int id = unbox_value<unsigned int>(cb.Tag());
                            _examController.Questions().GetAt(_examController.CurrentQuestion()).Answers().GetAt(id).IsChosen(cb.IsChecked().Value());
                        });

                    ContentStackPanel().Children().Append(answerCheckBox);

                    // Debug
                    if (answer.IsCorrect())
                    {
                        debugExamAnswersString << debugExamAnswersStringItemSeparator << answer.Text();
                        debugExamAnswersStringItemSeparator = L", ";
                    }
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
                    answerRadioButton.IsChecked(answer.IsChosen());

                    radioButtons.Items().Append(answerRadioButton);

                    // Debug
                    if (answer.IsCorrect())
                    {
                        debugExamAnswersString << debugExamAnswersStringItemSeparator << answer.Text();
                        debugExamAnswersStringItemSeparator = L", ";
                    }
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
                textBox.Margin({ 0, 8, 0, 0 });
                textBox.PlaceholderText(L"Ответ");

                const auto storedAnswerText = _examController.Questions().GetAt(_examController.CurrentQuestion()).CustomDataContext();

                if (storedAnswerText != nullptr)
                    textBox.Text(unbox_value<hstring>(storedAnswerText));

                textBox.LostFocus([this](IInspectable const& sender, RoutedEventArgs const&)
                    {
                        if (_enterHandled)
                            return;

                        const auto tb = unbox_value<TextBox>(sender);

                        const auto userText = tb.Text().c_str();

                        const auto currentQuestion = _examController.Questions().GetAt(_examController.CurrentQuestion());

                        if (tb.Text() != L"")
                            currentQuestion.CustomDataContext(box_value(tb.Text()));
                        else
                            currentQuestion.CustomDataContext(nullptr);

                        for (auto answer : currentQuestion.Answers())
                        {
                            const auto answerText = answer.Text().c_str();

                            // ReSharper disable once CppZeroConstantCanBeReplacedWithNullptr
                            if (CSTR_EQUAL == CompareStringEx(LOCALE_NAME_USER_DEFAULT, LINGUISTIC_IGNORECASE, answerText, -1, userText, -1, nullptr, nullptr, 0))
                                answer.IsChosen(true);
                            else
                                answer.IsChosen(false);
                        }
                    });

                textBox.KeyDown([this](IInspectable const& sender, KeyRoutedEventArgs const& e)
                    {
                        if (_enterHandled || e.Key() != VirtualKey::Enter)
                            return;

                        _enterHandled = true;

                        const auto tb = unbox_value<TextBox>(sender);

                        const auto userText = tb.Text().c_str();

                        const auto currentQuestion = _examController.Questions().GetAt(_examController.CurrentQuestion());

                        if (tb.Text() != L"")
                            currentQuestion.CustomDataContext(box_value(tb.Text()));
                        else
                            currentQuestion.CustomDataContext(nullptr);

                        for (auto answer : currentQuestion.Answers())
                        {
                            const auto answerText = answer.Text().c_str();

                            // ReSharper disable once CppZeroConstantCanBeReplacedWithNullptr
                            if (CSTR_EQUAL == CompareStringEx(LOCALE_NAME_USER_DEFAULT, LINGUISTIC_IGNORECASE, answerText, -1, userText, -1, nullptr, nullptr, 0))
                                answer.IsChosen(true);
                            else
                                answer.IsChosen(false);
                        }

                        _examController.CallControl(ExamControlAction::ContinueForward);
                    });

                ContentStackPanel().Children().Append(textBox);

                // Debug
                for (const auto debugAnswers = _examController.Questions().GetAt(_examController.CurrentQuestion()).Answers();
                    auto answer : debugAnswers)
                {
                    if (answer.IsCorrect())
                    {
                        debugExamAnswersString << debugExamAnswersStringItemSeparator << answer.Text();
                        debugExamAnswersStringItemSeparator = L", ";
                    }
                }

                break;
            }
        }

        // Debug
        if (__debugExamAnswers)
        {
            DebugExamAnswersTextBlock().Text(debugExamAnswersString.str());
            DebugExamAnswersTextBlock().Visibility(Visibility::Visible);
        }
    }
}
