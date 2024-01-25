#include "pch.h"

#include "ExamControlHostPage.xaml.h"
#if __has_include("ExamControlHostPage.g.cpp")
#include "ExamControlHostPage.g.cpp"
#endif

#include <sqlite3.h>
#include <random>
#include "WinUIEx.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::FiberOpticCommsCourseware::implementation
{
    ExamControlHostPage::ExamControlHostPage()
    {
        InitializeComponent();

        _examController = ExamController();
        // ReSharper disable once CppExpressionWithoutSideEffects
        _examController.OnControlAction({ this, &ExamControlHostPage::OnControlAction });
    }

    void ExamControlHostPage::OnNavigatedTo(NavigationEventArgs const& e)
    {
        _mainWindow = unbox_value<MainWindow>(e.Parameter());

        InfoBar().IsOpen(!_mainWindow.IsDataCurrent());

        // ReSharper disable once CppExpressionWithoutSideEffects
        ContentFrame().Navigate(xaml_typename<ExamStartPage>(), _examController);
    }

    // ReSharper disable CppExpressionWithoutSideEffects
    void ExamControlHostPage::OnControlAction(IInspectable const&, const ExamControlAction action)
    {
        // ReSharper disable once CppIncompleteSwitchStatement
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch (action)
        {
            case ExamControlAction::Start:
            {
                _mainWindow.AreNavigationMenuItemsEnabled(false);

                LoadQuestions();

                ContentFrame().Navigate(xaml_typename<ExamQuestionHostPage>(), _examController);
                break;
            }
            case ExamControlAction::End:
            {
                _mainWindow.AreNavigationMenuItemsEnabled(true);

                ContentFrame().Navigate(xaml_typename<ExamResultsPage>(), _examController);

                break;
            }
            case ExamControlAction::Reset:
            {
                _mainWindow.AreNavigationMenuItemsEnabled(true);
        
                _examController = ExamController();
                _examController.OnControlAction({ this, &ExamControlHostPage::OnControlAction });
        
                ContentFrame().Navigate(xaml_typename<ExamStartPage>(), _examController);
        
                break;
            }
        }
    }
    // ReSharper restore CppExpressionWithoutSideEffects

    // TODO: Move out elsewhere
#define QUICK_SQL_QUERY(db, statementName, query, body, additionalErrorActions)                      \
    sqlite3_stmt* statementName;                                                                     \
    int statementName##_resultCode = sqlite3_prepare_v2(db, query, -1, &(statementName), nullptr);   \
                                                                                                     \
    if (statementName##_resultCode != SQLITE_OK)                                                     \
    {                                                                                                \
        std::string s = "Не удалось подготовить запрос для базы данных: ";                           \
        s += sqlite3_errmsg(db);                                                                     \
        co_await WinUIEx::ShowSimpleContentDialog(*this, L"Ошибка базы данных", to_hstring(s));      \
        sqlite3_finalize(statementName);                                                             \
        additionalErrorActions                                                                       \
        sqlite3_close(db);                                                                           \
        Application::Current().Exit();                                                               \
        co_return;                                                                                   \
    }                                                                                                \
                                                                                                     \
    while ((statementName##_resultCode = sqlite3_step(statementName)) == SQLITE_ROW)                 \
        body                                                                                         \
                                                                                                     \
    if (statementName##_resultCode != SQLITE_DONE)                                                   \
    {                                                                                                \
        std::string s = "Ошибка базы данных: ";                                                      \
        s += sqlite3_errmsg(db);                                                                     \
        co_await WinUIEx::ShowSimpleContentDialog(*this, L"Ошибка базы данных", to_hstring(s));      \
        sqlite3_finalize(statementName);                                                             \
        additionalErrorActions                                                                       \
        sqlite3_close(db);                                                                           \
        Application::Current().Exit();                                                               \
        co_return;                                                                                   \
    }                                                                                                \
                                                                                                     \
    sqlite3_finalize(statementName);

    IAsyncAction ExamControlHostPage::LoadQuestions() const
    {
        sqlite3* db;
        int resultCode = sqlite3_open_v2("data.db", &db, SQLITE_OPEN_READONLY, nullptr);
        if (resultCode != SQLITE_OK)
        {
            std::string s = "Не удалось открыть базу данных: ";
            s += sqlite3_errmsg(db);
            co_await WinUIEx::ShowSimpleContentDialog(*this, L"Ошибка базы данных", to_hstring(s));
            sqlite3_close(db);
            Application::Current().Exit();
            co_return;
        }

        sqlite3_stmt* questionsSqlStatement;
        resultCode = sqlite3_prepare_v2(db, "SELECT id, question_text, type FROM exam_questions;", -1, &questionsSqlStatement, nullptr);
        if (resultCode != SQLITE_OK)
        {
            std::string s = "Не удалось подготовить запрос для базы данных: ";
            s += sqlite3_errmsg(db);
            co_await WinUIEx::ShowSimpleContentDialog(*this, L"Ошибка базы данных", to_hstring(s));
            sqlite3_close(db);
            Application::Current().Exit();
            co_return;
        }

        while ((resultCode = sqlite3_step(questionsSqlStatement)) == SQLITE_ROW)
        {
            const int64_t questionId = sqlite3_column_int64(questionsSqlStatement, 0);
            const auto* questionText = static_cast<const wchar_t*>(sqlite3_column_text16(questionsSqlStatement, 1));
            // ReSharper disable once CppTooWideScope
            auto questionType = static_cast<ExamQuestionType>(sqlite3_column_int(questionsSqlStatement, 2));

            switch (questionType)
            {
                case ExamQuestionType::MultipleChoice:
                case ExamQuestionType::SingleChoice:
                {
                    ExamChoiceQuestion currentQuestion(questionId, questionText, questionType);

                    const auto answersSql = "SELECT answer_text, is_correct FROM exam_choice_answers WHERE question_id = " + std::to_string(questionId);

                    QUICK_SQL_QUERY(db, answersSqlStatement, answersSql.c_str(),
                        {
                            const auto* answerText = static_cast<const wchar_t*>(sqlite3_column_text16(answersSqlStatement, 0));
                            const bool answerCorrect = sqlite3_column_int(answersSqlStatement, 1);

                            ExamChoiceAnswer answer(answerText, answerCorrect);
                            currentQuestion.Answers().Append(answer);
                        },
                        {
                            sqlite3_finalize(questionsSqlStatement);
                        });

                    VectorUtils::ShuffleVector(currentQuestion.Answers().as<Collections::IVector<ExamChoiceAnswer>>());

                    _examController.Questions().Append(currentQuestion);

                    break;
                }
                case ExamQuestionType::FreeInput:
                {
                    ExamFreeInputQuestion currentQuestion(questionId, questionText);

                    const auto answersSql = "SELECT answer_text FROM exam_free_input_answers WHERE question_id = " + std::to_string(questionId);

                    QUICK_SQL_QUERY(db, answersSqlStatement, answersSql.c_str(),
                        {
                            const auto* answerText = static_cast<const wchar_t*>(sqlite3_column_text16(answersSqlStatement, 0));

                            currentQuestion.CorrectAnswers().Append(answerText);
                        },
                        {
                            sqlite3_finalize(questionsSqlStatement);
                        });

                    _examController.Questions().Append(currentQuestion);

                    break;
                }
                case ExamQuestionType::Ordering:
                {
                    ExamOrderingQuestion currentQuestion(questionId, questionText);

                    const auto answersSql = "SELECT answer_text, correct_order FROM exam_ordering_answers WHERE question_id = " + std::to_string(questionId);

                    QUICK_SQL_QUERY(db, answersSqlStatement, answersSql.c_str(),
                        {
                            const auto* answerText = static_cast<const wchar_t*>(sqlite3_column_text16(answersSqlStatement, 0));
                            const int correctOrder = sqlite3_column_int(answersSqlStatement, 1);

                            ExamOrderingAnswer answer(answerText, correctOrder);
                            currentQuestion.Answers().Append(answer);
                        },
                        {
                            sqlite3_finalize(questionsSqlStatement);
                        });

                    VectorUtils::ShuffleVector(currentQuestion.Answers().as<Collections::IVector<ExamOrderingAnswer>>());

                    _examController.Questions().Append(currentQuestion);

                    break;
                }
                case ExamQuestionType::SelectionInRange:
                {
                    double minimum = 0, maximum = 0, step = 0, correctAnswer = 0;

                    const auto answersSql = "SELECT minimum, maximum, step, correct_answer FROM exam_selection_in_range_properties WHERE question_id = " + std::to_string(questionId);

                    QUICK_SQL_QUERY(db, answersSqlStatement, answersSql.c_str(),
                        {
                            minimum = sqlite3_column_double(answersSqlStatement, 0);
                            maximum = sqlite3_column_double(answersSqlStatement, 1);
                            step = sqlite3_column_double(answersSqlStatement, 2);
                            correctAnswer = sqlite3_column_double(answersSqlStatement, 3);
                        },
                        {
                            sqlite3_finalize(questionsSqlStatement);
                        });

                    ExamSelectionInRangeQuestion currentQuestion(questionId, questionText, minimum, maximum, step, correctAnswer);
                    _examController.Questions().Append(currentQuestion);

                    break;
                }
                case ExamQuestionType::Classification:
                {
                    ExamClassificationQuestion currentQuestion(questionId, questionText);

                    const auto answersSql = "SELECT answer_text, correct_category_id FROM exam_classification_answers WHERE question_id = " + std::to_string(questionId);

                    QUICK_SQL_QUERY(db, answersSqlStatement, answersSql.c_str(),
                        {
                            const auto* answerText = static_cast<const wchar_t*>(sqlite3_column_text16(answersSqlStatement, 0));
                            const int correctCategoryId = sqlite3_column_int(answersSqlStatement, 1);

                            ExamClassificationAnswer answer(answerText, correctCategoryId);
                            currentQuestion.Answers().Append(answer);
                        },
                        {
                            sqlite3_finalize(questionsSqlStatement);
                        });

                    VectorUtils::ShuffleVector(currentQuestion.Answers());

                    const auto categoriesSql = "SELECT category_name FROM exam_classification_categories WHERE question_id = " + std::to_string(questionId) + " ORDER BY category_id";

                    QUICK_SQL_QUERY(db, categoriesSqlStatement, categoriesSql.c_str(),
                        {
                            const auto* categoryText = static_cast<const wchar_t*>(sqlite3_column_text16(categoriesSqlStatement, 0));

                            currentQuestion.Categories().Append(categoryText);
                        },
                        {
                            sqlite3_finalize(questionsSqlStatement);
                        });

                    _examController.Questions().Append(currentQuestion);

                    break;
                }
            }
        }

        VectorUtils::ShuffleVector(_examController.Questions());

        if (resultCode != SQLITE_DONE)
        {
            std::string s = "Ошибка базы данных: ";
            s += sqlite3_errmsg(db);
            co_await WinUIEx::ShowSimpleContentDialog(*this, L"Ошибка базы данных", to_hstring(s));
        }

        sqlite3_finalize(questionsSqlStatement);
        sqlite3_close(db);
    }
}
