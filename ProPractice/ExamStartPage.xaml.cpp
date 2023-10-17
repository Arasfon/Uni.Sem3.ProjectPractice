#include "pch.h"
#include "ExamStartPage.xaml.h"

#if __has_include("ExamStartPage.g.cpp")
#include "ExamStartPage.g.cpp"
#endif
#include "WinUIEx.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::Foundation;

namespace winrt::ProPractice::implementation
{
    ExamStartPage::ExamStartPage()
    {
        InitializeComponent();
    }

    IAsyncAction ExamStartPage::OnNavigatedTo(Navigation::NavigationEventArgs const& e)
    {
        _examController = unbox_value<ExamController>(e.Parameter());

        int64_t questionCount = co_await GetQuestionCount();
        ExamRulesTitleTextBlock().Text(to_hstring(std::vformat(to_string(ExamRulesTitleTextBlock().Text()), std::make_format_args(questionCount))));
    }

    IAsyncOperation<int64_t> ExamStartPage::GetQuestionCount() const
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
            co_return -1;
        }

        sqlite3_stmt* sqlStatement;
        const auto sql = "SELECT COUNT(*) FROM exam_questions;";
        resultCode = sqlite3_prepare_v2(db, sql, -1, &sqlStatement, nullptr);
        if (resultCode != SQLITE_OK)
        {
            std::string s = "Не удалось подготовить запрос для базы данных: ";
            s += sqlite3_errmsg(db);
            co_await WinUIEx::ShowSimpleContentDialog(*this, L"Ошибка базы данных", to_hstring(s));
            sqlite3_close(db);
            Application::Current().Exit();
            co_return -1;
        }

        int64_t count;

        while ((resultCode = sqlite3_step(sqlStatement)) == SQLITE_ROW)
        {
            count = sqlite3_column_int64(sqlStatement, 0);
        }

        if (resultCode != SQLITE_DONE)
        {
            std::string s = "Ошибка базы данных: ";
            s += sqlite3_errmsg(db);
            co_await WinUIEx::ShowSimpleContentDialog(*this, L"Ошибка базы данных", to_hstring(s));
        }

        sqlite3_finalize(sqlStatement);
        sqlite3_close(db);

        co_return count;
    }

    void ExamStartPage::StartButton_Click(IInspectable const&, RoutedEventArgs const&) const
    {
        _examController.CallControl(ExamControlAction::Start);
    }
}
