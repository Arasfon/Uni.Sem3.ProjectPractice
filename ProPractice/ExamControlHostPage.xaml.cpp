#include "pch.h"

#include "ExamControlHostPage.xaml.h"
#if __has_include("ExamControlHostPage.g.cpp")
#include "ExamControlHostPage.g.cpp"
#endif

#include <sqlite3.h>
#include <random>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::ProPractice::implementation
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

        // ReSharper disable once CppExpressionWithoutSideEffects
        ContentFrame().Navigate(xaml_typename<ExamStartPage>(), _examController);
    }

    // ReSharper disable CppExpressionWithoutSideEffects
    void ExamControlHostPage::OnControlAction(IInspectable const&, const ExamControlAction action)
    {
        switch (action)
        {
            case ExamControlAction::Start:
            {
                _mainWindow.AreNavigationMenuItemsEnabled(false);

                LoadQuestions();

                ContentFrame().Navigate(xaml_typename<ExamQuestionHostPage>(), _examController);
                break;
            }
            case ExamControlAction::Continue:
            {
                if (_examController.CurrentQuestion() + 1 != _examController.Questions().Size())
                    break;

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

    IAsyncAction ExamControlHostPage::LoadQuestions()
    {
        sqlite3* db;
        int resultCode = sqlite3_open_v2("data.db", &db, SQLITE_OPEN_READONLY, nullptr);
        if (resultCode != SQLITE_OK)
        {
            std::string s = "Не удалось открыть базу данных: ";
            s += sqlite3_errmsg(db);
            co_await ShowErrorContentDialog(L"Ошибка базы данных", to_hstring(s));
            sqlite3_close(db);
            Application::Current().Exit();
            co_return;
        }

        sqlite3_stmt* sqlStatement;
        const auto sql = "SELECT exam_questions.id as question_id, question_text, type as question_type, answer_text, is_correct as is_answer_correct FROM exam_questions INNER JOIN exam_answers ON exam_answers.question_id = exam_questions.id ORDER BY exam_questions.id, exam_answers.id;";
        resultCode = sqlite3_prepare_v2(db, sql, -1, &sqlStatement, nullptr);
        if (resultCode != SQLITE_OK)
        {
            std::string s = "Не удалось подготовить запрос для базы данных: ";
            s += sqlite3_errmsg(db);
            co_await ShowErrorContentDialog(L"Ошибка базы данных", to_hstring(s));
            sqlite3_close(db);
            Application::Current().Exit();
            co_return;
        }

        int64_t currentId = -1;
        ExamQuestion currentQuestion = nullptr;

        while ((resultCode = sqlite3_step(sqlStatement)) == SQLITE_ROW)
        {
            const int64_t questionId = sqlite3_column_int64(sqlStatement, 0);

            if (currentId != questionId)
            {
                if (currentQuestion != nullptr)
                {
                    ShuffleVector(currentQuestion.Answers());
                    _examController.Questions().Append(currentQuestion);
                }

                currentQuestion = ExamQuestion();
                const auto* questionText = static_cast<const wchar_t*>(sqlite3_column_text16(sqlStatement, 1));
                currentQuestion.Text(questionText);
                auto questionType = static_cast<ExamQuestionType>(sqlite3_column_int(sqlStatement, 2));
                currentQuestion.Type(questionType);
            }

            ExamAnswer answer;
            const auto* answerText = static_cast<const wchar_t*>(sqlite3_column_text16(sqlStatement, 3));
            answer.Text(answerText);
            const bool answerCorrect = sqlite3_column_int(sqlStatement, 4);
            answer.IsCorrect(answerCorrect);

            currentQuestion.Answers().Append(answer);

            currentId = questionId;
        }

        if (currentQuestion != nullptr)
        {
            ShuffleVector(currentQuestion.Answers());
            _examController.Questions().Append(currentQuestion);
        }

        ShuffleVector(_examController.Questions());

        if (resultCode != SQLITE_DONE)
        {
            std::string s = "Ошибка базы данных: ";
            s += sqlite3_errmsg(db);
            co_await ShowErrorContentDialog(L"Ошибка базы данных", to_hstring(s));
        }

        sqlite3_finalize(sqlStatement);
        sqlite3_close(db);
    }

    template <typename T>
    void ExamControlHostPage::ShuffleVector(Collections::IVector<T> const& vector)
    {
        std::mt19937 randomEngine(_randomDevice());

        for (unsigned int i = vector.Size() - 1; i > 0; --i)
        {
            std::uniform_int_distribution<> distribution(0, i);
            SwapVectorItems(vector, i, distribution(randomEngine));
        }
    }

    template <typename T>
    void ExamControlHostPage::SwapVectorItems(Collections::IVector<T> const& vector, unsigned int firstIndex, unsigned int secondIndex)
    {
        auto item = vector.GetAt(firstIndex);
        vector.SetAt(firstIndex, vector.GetAt(secondIndex));
        vector.SetAt(secondIndex, item);
    }

    IAsyncAction ExamControlHostPage::ShowErrorContentDialog(hstring const& title, hstring const& content) const
    {
        const Controls::ContentDialog dialog;
        dialog.XamlRoot(this->XamlRoot());
        dialog.Style(unbox_value<Microsoft::UI::Xaml::Style>(Application::Current().Resources().Lookup(box_value(L"DefaultContentDialogStyle"))));
        dialog.Title(box_value(title));
        dialog.Content(box_value(content));
        dialog.CloseButtonText(L"Ок");

        co_await dialog.ShowAsync();
    }
}
