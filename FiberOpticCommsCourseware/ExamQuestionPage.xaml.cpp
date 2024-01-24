#include "pch.h"

#include "ExamQuestionPage.xaml.h"
#if __has_include("ExamQuestionPage.g.cpp")
#include "ExamQuestionPage.g.cpp"
#endif

#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <sstream>

#include <winrt/Windows.ApplicationModel.DataTransfer.h>

#include "Debug.h"

#undef min
#include <algorithm>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Input;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Windows::System;

namespace winrt::FiberOpticCommsCourseware::implementation
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

                const ExamChoiceQuestion castQuestion = question.as<ExamChoiceQuestion>();

                if (castQuestion.Answers().Size() >= 8)
                {
                    const ListView listView;

                    listView.VerticalAlignment(VerticalAlignment::Stretch);
                    listView.HorizontalAlignment(HorizontalAlignment::Stretch);
                    listView.BorderThickness({ 1, 1, 1, 1 });
                    listView.BorderBrush(unbox_value<Media::Brush>(Application::Current().Resources().Lookup(box_value(L"SystemControlForegroundBaseMediumLowBrush"))));
                    listView.ItemTemplate(unbox_value<DataTemplate>(Resources().Lookup(box_value(L"ChoiceQuestionListViewItemTemplate"))));
                    listView.ItemsSource(castQuestion.Answers());
                    listView.SelectionMode(ListViewSelectionMode::Multiple);

                    for (auto answer : castQuestion.Answers())
                    {
                        if (answer.Chosen())
                            listView.SelectedItems().Append(answer);
                    }

                    BaseContentPresenter().Content(listView);

                    listView.SelectionChanged([this](IInspectable const&, SelectionChangedEventArgs const& e)
                        {
                            if (e.RemovedItems().Size() > 0)
                            {
                                for (auto boxedAnswer : e.RemovedItems())
                                {
                                    auto answer = unbox_value<ExamChoiceAnswer>(boxedAnswer);
                                    answer.Chosen(false);
                                }
                            }

                            if (e.AddedItems().Size() > 0)
                            {
                                for (auto boxedAnswer : e.AddedItems())
                                {
                                    auto answer = unbox_value<ExamChoiceAnswer>(boxedAnswer);
                                    answer.Chosen(true);
                                }
                            }
                        });

                    // Debug ↓
                    for (unsigned int i = 0; i < castQuestion.Answers().Size(); i++)
                    {
                        if (auto answer = castQuestion.Answers().GetAt(i);
                            answer.Correct())
                        {
                            debugExamAnswersString << debugExamAnswersStringItemSeparator << answer.Text();
                            debugExamAnswersStringItemSeparator = L", ";
                        }
                    }
                    // Debug ↑
                }
                else
                {
                    for (unsigned int i = 0; i < castQuestion.Answers().Size(); i++)
                    {
                        auto answer = castQuestion.Answers().GetAt(i);

                        const CheckBox answerCheckBox;

                        answerCheckBox.Content(box_value(answer.Text()));
                        answerCheckBox.Tag(box_value(i));
                        answerCheckBox.IsChecked(answer.Chosen());
                        answerCheckBox.Click([this](IInspectable const& sender, RoutedEventArgs const&)
                            {
                                const CheckBox cb = unbox_value<CheckBox>(sender);
                                const int id = unbox_value<unsigned int>(cb.Tag());
                                _examController.Questions().GetAt(_examController.CurrentQuestion()).as<ExamChoiceQuestion>()
                                    .Answers().GetAt(id).Chosen(cb.IsChecked().Value());
                            });

                        InnerContentStackPanel().Children().Append(answerCheckBox);

                        // Debug ↓
                        if (answer.Correct())
                        {
                            debugExamAnswersString << debugExamAnswersStringItemSeparator << answer.Text();
                            debugExamAnswersStringItemSeparator = L", ";
                        }
                        // Debug ↑
                    }
                }

                break;
            }
            case ExamQuestionType::SingleChoice:
            {
                AnswerTypeTextBlock().Text(L"Выберите один из вариантов ответа:");

                const ExamChoiceQuestion castQuestion = question.as<ExamChoiceQuestion>();

                if (castQuestion.Answers().Size() >= 8)
                {
                    const ComboBox comboBox;

                    comboBox.PlaceholderText(L"Нет ответа");
                    comboBox.ItemTemplate(unbox_value<DataTemplate>(Resources().Lookup(box_value(L"ChoiceQuestionListViewItemTemplate"))));
                    comboBox.ItemsSource(castQuestion.Answers());

                    for (auto answer : castQuestion.Answers())
                    {
                        if (answer.Chosen())
                        {
                            comboBox.SelectedItem(answer);
                            break;
                        }
                    }

                    InnerContentStackPanel().Children().Append(comboBox);

                    comboBox.SelectionChanged([this](IInspectable const&, SelectionChangedEventArgs const& e)
                        {
                            if (e.RemovedItems().Size() > 0)
                            {
                                for (auto boxedAnswer : e.RemovedItems())
                                {
                                    auto answer = unbox_value<ExamChoiceAnswer>(boxedAnswer);
                                    answer.Chosen(false);
                                }
                            }

                            if (e.AddedItems().Size() > 0)
                            {
                                for (auto boxedAnswer : e.AddedItems())
                                {
                                    auto answer = unbox_value<ExamChoiceAnswer>(boxedAnswer);
                                    answer.Chosen(true);
                                }
                            }
                        });

                    // Debug ↓
                    for (unsigned int i = 0; i < castQuestion.Answers().Size(); i++)
                    {
                        if (auto answer = castQuestion.Answers().GetAt(i);
                            answer.Correct())
                        {
                            debugExamAnswersString << debugExamAnswersStringItemSeparator << answer.Text();
                            debugExamAnswersStringItemSeparator = L", ";
                        }
                    }
                    // Debug ↑
                }
                else
                {
                    const RadioButtons radioButtons;

                    for (unsigned int i = 0; i < castQuestion.Answers().Size(); i++)
                    {
                        auto answer = castQuestion.Answers().GetAt(i);

                        const RadioButton answerRadioButton;

                        answerRadioButton.Content(box_value(answer.Text()));
                        answerRadioButton.Tag(box_value(i));
                        answerRadioButton.IsChecked(answer.Chosen());

                        radioButtons.Items().Append(answerRadioButton);

                        // Debug ↓
                        if (answer.Correct())
                        {
                            debugExamAnswersString << debugExamAnswersStringItemSeparator << answer.Text();
                            debugExamAnswersStringItemSeparator = L", ";
                        }
                        // Debug ↑
                    }

                    radioButtons.SelectionChanged([this](IInspectable const&, SelectionChangedEventArgs const& e)
                        {
                            const auto deselectedItem = unbox_value<RadioButton>(e.RemovedItems().GetAt(0));
                            const auto selectedItem = unbox_value<RadioButton>(e.AddedItems().GetAt(0));

                            if (deselectedItem != nullptr)
                                _examController.Questions().GetAt(_examController.CurrentQuestion()).as<ExamChoiceQuestion>()
                                    .Answers().GetAt(unbox_value<unsigned int>(deselectedItem.Tag())).Chosen(false);
                            if (selectedItem != nullptr)
                                _examController.Questions().GetAt(_examController.CurrentQuestion()).as<ExamChoiceQuestion>()
                                    .Answers().GetAt(unbox_value<unsigned int>(selectedItem.Tag())).Chosen(true);
                        });

                    InnerContentStackPanel().Children().Append(radioButtons);
                }

                break;
            }
            case ExamQuestionType::FreeInput:
            {
                AnswerTypeTextBlock().Text(L"Введите свой ответ:");

                const ExamFreeInputQuestion castQuestion = question.as<ExamFreeInputQuestion>();

                const TextBox textBox;

                textBox.Width(300);
                textBox.PlaceholderText(L"Ответ");

                textBox.Text(castQuestion.UserAnswer());
                textBox.Tag(box_value(_examController.CurrentQuestion()));

                textBox.LostFocus([this](IInspectable const& sender, RoutedEventArgs const&)
                    {
                        if (_enterHandled)
                            return;

                        const auto tb = unbox_value<TextBox>(sender);

                        const auto questionId = unbox_value<uint32_t>(unbox_value<FrameworkElement>(sender).Tag());

                        const auto currentQuestion = _examController.Questions().GetAt(questionId).as<ExamFreeInputQuestion>();

                        currentQuestion.UserAnswer(tb.Text());
                    });

                textBox.KeyDown([this](IInspectable const& sender, KeyRoutedEventArgs const& e)
                    {
                        if (_enterHandled || e.Key() != VirtualKey::Enter)
                            return;

                        _enterHandled = true;

                        const auto tb = unbox_value<TextBox>(sender);

                        const auto questionId = unbox_value<uint32_t>(unbox_value<FrameworkElement>(sender).Tag());

                        const auto currentQuestion = _examController.Questions().GetAt(questionId).as<ExamFreeInputQuestion>();

                        currentQuestion.UserAnswer(tb.Text());

                        _examController.CallControl(ExamControlAction::ContinueForward);
                    });

                InnerContentStackPanel().Children().Append(textBox);

                // Debug ↓
                for (const auto debugAnswers = castQuestion.CorrectAnswers();
                        auto answer : debugAnswers)
                {
                    debugExamAnswersString << debugExamAnswersStringItemSeparator << answer;
                    debugExamAnswersStringItemSeparator = L", ";
                }
                // Debug ↑

                break;
            }
            case ExamQuestionType::Ordering:
            {
                AnswerTypeTextBlock().Text(L"Расположите ответы в правильном порядке:");

                const ExamOrderingQuestion castQuestion = question.as<ExamOrderingQuestion>();

                const ListView listView;

                listView.VerticalAlignment(VerticalAlignment::Stretch);
                listView.HorizontalAlignment(HorizontalAlignment::Stretch);
                listView.BorderThickness({ 1, 1, 1, 1 });
                listView.BorderBrush(unbox_value<Media::Brush>(Application::Current().Resources().Lookup(box_value(L"SystemControlForegroundBaseMediumLowBrush"))));
                listView.ItemTemplate(unbox_value<DataTemplate>(Resources().Lookup(box_value(L"OrderingQuestionListViewItemTemplate"))));
                listView.ItemsSource(castQuestion.Answers());
                listView.CanReorderItems(true);
                listView.AllowDrop(true);

                BaseContentPresenter().Content(listView);

                // Debug ↓
                for (const auto debugAnswers = castQuestion.Answers();
                    auto answer : debugAnswers)
                {
                    debugExamAnswersString << debugExamAnswersStringItemSeparator << answer.Text() << L" (" << answer.CorrectOrder() << L")";
                    debugExamAnswersStringItemSeparator = L", ";
                }
                // Debug ↑

                break;
            }
            case ExamQuestionType::SelectionInRange:
            {
                AnswerTypeTextBlock().Text(L"Выберите ответ:");

                const ExamSelectionInRangeQuestion castQuestion = question.as<ExamSelectionInRangeQuestion>();

                const Slider slider;

                slider.Width(300);
                slider.Margin({ 0, 8, 0, 0 });
                slider.VerticalAlignment(VerticalAlignment::Center);
                slider.Minimum(castQuestion.Minimum());
                slider.Maximum(castQuestion.Maximum());
                slider.StepFrequency(castQuestion.Step());
                slider.TickFrequency(castQuestion.Step());
                slider.TickPlacement(Primitives::TickPlacement::Outside);

                slider.ValueChanged([this](IInspectable const&, Primitives::RangeBaseValueChangedEventArgs const& e)
                    {
                        const auto currentQuestion = _examController.Questions().GetAt(_examController.CurrentQuestion()).as<ExamSelectionInRangeQuestion>();

                        currentQuestion.UserAnswer(e.NewValue());
                    });

                InnerContentStackPanel().Spacing(8);
                InnerContentStackPanel().VerticalAlignment(VerticalAlignment::Top);
                InnerContentStackPanel().Orientation(Orientation::Horizontal);
                auto tb = TextBlock();
                tb.Text(to_hstring(castQuestion.Minimum()));
                tb.VerticalAlignment(VerticalAlignment::Center);
                InnerContentStackPanel().Children().Append(tb);
                InnerContentStackPanel().Children().Append(slider);
                tb = TextBlock();
                tb.Text(to_hstring(castQuestion.Maximum()));
                tb.VerticalAlignment(VerticalAlignment::Center);
                InnerContentStackPanel().Children().Append(tb);

                // Debug ↓
                debugExamAnswersString << castQuestion.CorrectAnswer();
                // Debug ↑

                break;
            }
            case ExamQuestionType::Classification:
            {
                AnswerTypeTextBlock().Text(L"Распределите ответы по категориям.");

                const ExamClassificationQuestion castQuestion = question.as<ExamClassificationQuestion>();

                const Grid baseGrid;

                // Column definitions

                ColumnDefinition coldef;
                coldef.Width(GridLength { 1, GridUnitType::Star });
                baseGrid.ColumnDefinitions().Append(coldef);

                coldef = ColumnDefinition();
                coldef.Width(GridLength { 1, GridUnitType::Star });
                baseGrid.ColumnDefinitions().Append(coldef);

                // Drag'n'drop setup

                auto listViewDragOverHandler = [](IInspectable const&, DragEventArgs const& e)
                    {
                        e.AcceptedOperation(Windows::ApplicationModel::DataTransfer::DataPackageOperation::Move);
                    };

                auto listViewDragItemsStartingHandler = [this](IInspectable const&, DragItemsStartingEventArgs const& e)
                    {
                        uint32_t idx;
                        _examController.Questions().GetAt(_examController.CurrentQuestion()).as<ExamClassificationQuestion>().Answers().IndexOf(unbox_value<ExamClassificationAnswer>(e.Items().GetAt(0)), idx);
                        e.Data().SetText(to_hstring(idx));
                        e.Data().RequestedOperation(Windows::ApplicationModel::DataTransfer::DataPackageOperation::Move);
                    };

                auto listViewDropHandler = [this](const IInspectable sender, const DragEventArgs e) -> Windows::Foundation::IAsyncAction  // NOLINT(performance-unnecessary-value-param)
                    {
                        const auto targetListView = unbox_value<ListView>(sender);

                        if (e.DataView().Contains(Windows::ApplicationModel::DataTransfer::StandardDataFormats::Text()))
                        {
                            const auto deferral = e.GetDeferral();

                            const auto dataText = co_await e.DataView().GetTextAsync();

                            const uint32_t idx = static_cast<uint32_t>(std::stoul(to_string(dataText)));

                            const auto answer = _examController.Questions().GetAt(_examController.CurrentQuestion()).as<ExamClassificationQuestion>().Answers().GetAt(idx);

                            // Insertion index
                            uint32_t insertionIndex = 0;
                            
                            if (targetListView.Items().Size() > 0)
                            {
                                const auto sampleItem = targetListView.ContainerFromIndex(0).as<ListViewItem>();
                                const double itemHeight = sampleItem.ActualHeight() + sampleItem.Margin().Top + sampleItem.Margin().Bottom;
                                const Windows::Foundation::Point dropPos = e.GetPosition(targetListView.ItemsPanelRoot());
                                insertionIndex = std::min(targetListView.Items().Size() - 1, static_cast<uint32_t>(dropPos.Y / itemHeight));
                                const ListViewItem targetItem = targetListView.ContainerFromIndex(insertionIndex).as<ListViewItem>();
                                const Windows::Foundation::Point positionInItem = e.GetPosition(targetItem);
                                if (positionInItem.Y > itemHeight / 2)
                                    insertionIndex++;
                                insertionIndex = std::min(targetListView.Items().Size(), insertionIndex);
                            }
                            
                            targetListView.Items().InsertAt(insertionIndex, answer);

                            answer.UserCategoryId(unbox_value<int32_t>(targetListView.Tag()));

                            _validDropReceived = true;

                            e.AcceptedOperation(Windows::ApplicationModel::DataTransfer::DataPackageOperation::Move);
                            deferral.Complete();
                        }
                    };

                auto listViewDragItemsCompletedHandler = [this](ListViewBase const& sender, DragItemsCompletedEventArgs const& e)
                    {
                        if (_validDropReceived && e.DropResult() == Windows::ApplicationModel::DataTransfer::DataPackageOperation::Move)
                        {
                            _validDropReceived = false;

                            for (auto answerBoxed : e.Items())
                            {
                                auto answer = unbox_value<ExamClassificationAnswer>(answerBoxed);
                                uint32_t answerIndex;
                                sender.Items().IndexOf(answer, answerIndex);
                                sender.Items().RemoveAt(answerIndex);
                            }
                        }
                    };

                // All answers ListView

                const ListView notClassifiedListView;

                Grid::SetColumn(notClassifiedListView, 0);
                notClassifiedListView.Margin({ 0, 0, 8, 0 });
                notClassifiedListView.VerticalAlignment(VerticalAlignment::Stretch);
                notClassifiedListView.HorizontalAlignment(HorizontalAlignment::Stretch);
                notClassifiedListView.BorderThickness({ 1, 1, 1, 1 });
                notClassifiedListView.BorderBrush(unbox_value<Media::Brush>(Application::Current().Resources().Lookup(box_value(L"SystemControlForegroundBaseMediumLowBrush"))));
                notClassifiedListView.ItemTemplate(unbox_value<DataTemplate>(Resources().Lookup(box_value(L"ClassificationQuestionListViewItemTemplate"))));
                notClassifiedListView.CanReorderItems(true);
                notClassifiedListView.CanDragItems(true);
                notClassifiedListView.AllowDrop(true);
                // ReSharper disable once CppExpressionWithoutSideEffects
                notClassifiedListView.DragOver(listViewDragOverHandler);
                // ReSharper disable once CppExpressionWithoutSideEffects
                notClassifiedListView.DragItemsStarting(listViewDragItemsStartingHandler);
                // ReSharper disable once CppExpressionWithoutSideEffects
                notClassifiedListView.Drop(listViewDropHandler);
                // ReSharper disable once CppExpressionWithoutSideEffects
                notClassifiedListView.DragItemsCompleted(listViewDragItemsCompletedHandler);
                notClassifiedListView.Tag(box_value(static_cast<int32_t>(-1)));

                // Fill answers
                for (uint32_t i = 0; i < castQuestion.Answers().Size(); i++)
                {
                    auto answer = castQuestion.Answers().GetAt(i);

                    if (answer.UserCategoryId() != -1)
                        continue;

                    notClassifiedListView.Items().Append(answer);
                }

                baseGrid.Children().Append(notClassifiedListView);

                // Arrows (?)

                // Categories' ListViews

                const StackPanel categoriesStackPanel;
                Grid::SetColumn(categoriesStackPanel, 1);
                categoriesStackPanel.Orientation(Orientation::Vertical);
                categoriesStackPanel.Spacing(8);
                categoriesStackPanel.HorizontalAlignment(HorizontalAlignment::Stretch);

                // Generate categories
                for (uint32_t i = 0; i < castQuestion.Categories().Size(); i++)
                {
                    const TextBlock header;
                    header.Text(castQuestion.Categories().GetAt(i));
                    header.HorizontalAlignment(HorizontalAlignment::Center);
                    header.Style(unbox_value<Microsoft::UI::Xaml::Style>(Application::Current().Resources().Lookup(box_value(L"BodyStrongTextBlockStyle"))));
                    categoriesStackPanel.Children().Append(header);

                    const ListView categoryListView;
                    categoryListView.Height(200);
                    categoryListView.HorizontalAlignment(HorizontalAlignment::Stretch);
                    categoryListView.BorderThickness({ 1, 1, 1, 1 });
                    categoryListView.BorderBrush(unbox_value<Media::Brush>(Application::Current().Resources().Lookup(box_value(L"SystemControlForegroundBaseMediumLowBrush"))));
                    categoryListView.ItemTemplate(unbox_value<DataTemplate>(Resources().Lookup(box_value(L"ClassificationQuestionListViewItemTemplate"))));
                    categoryListView.CanReorderItems(true);
                    categoryListView.CanDragItems(true);
                    categoryListView.AllowDrop(true);
                    // ReSharper disable once CppExpressionWithoutSideEffects
                    categoryListView.DragOver(listViewDragOverHandler);
                    // ReSharper disable once CppExpressionWithoutSideEffects
                    categoryListView.DragItemsStarting(listViewDragItemsStartingHandler);
                    // ReSharper disable once CppExpressionWithoutSideEffects
                    categoryListView.Drop(listViewDropHandler);
                    // ReSharper disable once CppExpressionWithoutSideEffects
                    categoryListView.DragItemsCompleted(listViewDragItemsCompletedHandler);
                    categoryListView.Tag(box_value(static_cast<int32_t>(i)));

                    // Fill answers
                    for (uint32_t j = 0; j < castQuestion.Answers().Size(); j++)
                    {
                        auto answer = castQuestion.Answers().GetAt(j);

                        if (std::cmp_not_equal(answer.UserCategoryId(), i))
                            continue;

                        categoryListView.Items().Append(answer);
                    }

                    categoriesStackPanel.Children().Append(categoryListView);
                }

                baseGrid.Children().Append(categoriesStackPanel);

                OuterContentScrollViewer().Content(baseGrid);

                // Debug ↓
                for (const auto debugAnswers = castQuestion.Answers();
                    auto answer : debugAnswers)
                {
                    debugExamAnswersString << debugExamAnswersStringItemSeparator << answer.Text() << L" (" << answer.CorrectCategoryId() << L")";
                    debugExamAnswersStringItemSeparator = L", ";
                }
                // Debug ↑

                break;
            }
        }

        // Debug ↓
        if (Debug::ShowExamAnswers)
        {
            DebugExamAnswersTextBlock().Text(debugExamAnswersString.str());
            DebugExamAnswersTextBlock().Visibility(Visibility::Visible);
        }
        // Debug ↑
    }
}
