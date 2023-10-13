#include "pch.h"

#include "TheoryPage.xaml.h"
#if __has_include("TheoryPage.g.cpp")
#include "TheoryPage.g.cpp"
#endif
#include "TheoryChapter.h"

#include <winrt/Microsoft.Web.WebView2.Core.h>
#include <winrt/Windows.Storage.Streams.h>
#include "sqlite3.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::Web::WebView2::Core;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation::Collections;

namespace winrt::ProPractice::implementation
{
    TheoryPage::TheoryPage()
    {
        InitializeComponent();

        _theoryChapters = winrt::single_threaded_observable_vector<ProPractice::TheoryChapter>();

        // ReSharper disable CppExpressionWithoutSideEffects
        ContentWebView().DefaultBackgroundColor(Windows::UI::Color(0, 255, 255, 255));
        ContentWebView().CoreWebView2Initialized({ this, &TheoryPage::OnWebViewInitialized });
        ContentWebView().EnsureCoreWebView2Async();
        // ReSharper restore CppExpressionWithoutSideEffects
    }

    IObservableVector<ProPractice::TheoryChapter> TheoryPage::TheoryChapters()
    {
        return _theoryChapters;
    }

    void TheoryPage::OnNavigatedFrom(NavigationEventArgs const& e)
    {
        ContentWebView().Close();
    }

    IAsyncAction TheoryPage::OnWebViewInitialized(WebView2 const& sender, CoreWebView2InitializedEventArgs const& e)
    {
        bool webViewInitializedSuccessfully = true;
        hstring webViewInitializationError;

        try
        {
            check_hresult(e.Exception());
        }
        catch (std::exception& ex)
        {
            webViewInitializedSuccessfully = false;
            webViewInitializationError = to_hstring(ex.what());
        }
        catch (hresult_error& err)
        {
            webViewInitializedSuccessfully = false;
            webViewInitializationError = to_hstring(err.message());
        }

        if (!webViewInitializedSuccessfully)
        {
            co_await ShowErrorContentDialog(L"Ошибка инициализации WebView2", L"Не удалось запустить WebView2: " + webViewInitializationError);
            Application::Current().Exit();
            co_return;
        }

#ifndef _DEBUG
        sender.CoreWebView2().Settings().AreBrowserAcceleratorKeysEnabled(false);
        sender.CoreWebView2().Settings().AreDefaultContextMenusEnabled(false);
        sender.CoreWebView2().Settings().AreDevToolsEnabled(false);
#endif
        sender.CoreWebView2().Settings().HiddenPdfToolbarItems(
            CoreWebView2PdfToolbarItems::Bookmarks |
            CoreWebView2PdfToolbarItems::Print |
            CoreWebView2PdfToolbarItems::Save |
            CoreWebView2PdfToolbarItems::SaveAs |
            CoreWebView2PdfToolbarItems::FullScreen |
            CoreWebView2PdfToolbarItems::MoreSettings | 
            CoreWebView2PdfToolbarItems::PageLayout);

        ContentWebView().CoreWebView2().AddWebResourceRequestedFilter(L"*", CoreWebView2WebResourceContext::All);
        // ReSharper disable once CppExpressionWithoutSideEffects
        ContentWebView().CoreWebView2().WebResourceRequested({ this, &TheoryPage::OnWebViewWebResourceRequested });

        co_await LoadChapters();
    }

    void TheoryPage::NavView_SelectionChanged(IInspectable const& sender, NavigationViewSelectionChangedEventArgs const& args)
    {
        ContentWebView().CoreWebView2().Navigate(L"https://theory.data/");
    }

    IAsyncAction TheoryPage::OnWebViewWebResourceRequested(CoreWebView2 const& sender, CoreWebView2WebResourceRequestedEventArgs const& e)
    {
        if (e.Request().Uri() != L"https://theory.data/")
        {
            e.Response(ContentWebView().CoreWebView2().Environment().CreateWebResourceResponse(nullptr, 404, L"Not Found", L""));
            co_return;
        }

        const InMemoryRandomAccessStream stream;
        const DataWriter dataWriter { stream };
        dataWriter.UnicodeEncoding(UnicodeEncoding::Utf8);

        sqlite3* db;
        int resultCode = sqlite3_open_v2("data.db", &db, SQLITE_OPEN_READONLY, nullptr);
        if (resultCode != SQLITE_OK) {
            std::string s = "Не удалось открыть базу данных: ";
            s += sqlite3_errmsg(db);
            co_await ShowErrorContentDialog(L"Ошибка базы данных", to_hstring(s));
            sqlite3_close(db);
            //Application::Current().Exit();
            e.Response(ContentWebView().CoreWebView2().Environment().CreateWebResourceResponse(nullptr, 502, L"Bad Gateway", L""));
            co_return;
        }

        sqlite3_stmt* sqlStatement;
        const auto sql = "SELECT content FROM book_chapters WHERE id = " +
            std::to_string(unbox_value<ProPractice::TheoryChapter>(NavView().SelectedItem()).Id()) +
            " LIMIT 1";
        resultCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &sqlStatement, nullptr);
        if (resultCode != SQLITE_OK) {
            std::string s = "Не удалось подготовить запрос для базы данных: ";
            s += sqlite3_errmsg(db);
            co_await ShowErrorContentDialog(L"Ошибка базы данных", to_hstring(s));
            sqlite3_close(db);
            //Application::Current().Exit();
            e.Response(ContentWebView().CoreWebView2().Environment().CreateWebResourceResponse(nullptr, 502, L"Bad Gateway", L""));
            co_return;
        }

        while ((resultCode = sqlite3_step(sqlStatement)) == SQLITE_ROW) {
            const void* content = sqlite3_column_blob(sqlStatement, 0);
            const int contentLength = sqlite3_column_bytes(sqlStatement, 0);

            dataWriter.WriteBytes(array_view(static_cast<const uint8_t*>(content), contentLength));
            co_await dataWriter.StoreAsync();
        }

        if (resultCode != SQLITE_DONE) {
            std::string s = "Ошибка базы данных: ";
            s += sqlite3_errmsg(db);
            co_await ShowErrorContentDialog(L"Ошибка базы данных", to_hstring(s));
        }

        sqlite3_finalize(sqlStatement);
        sqlite3_close(db);

        const CoreWebView2WebResourceResponse response = ContentWebView().CoreWebView2().Environment()
            .CreateWebResourceResponse(stream, 200, L"OK", L"Content-Type: application/pdf");
        e.Response(response);
    }

    IAsyncAction TheoryPage::LoadChapters()
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
        const auto sql = "SELECT id, title FROM book_chapters ORDER BY parent_id ASC, id ASC;";
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

        while ((resultCode = sqlite3_step(sqlStatement)) == SQLITE_ROW)
        {
            int64_t id = sqlite3_column_int64(sqlStatement, 0);
            const auto* title = static_cast<const wchar_t*>(sqlite3_column_text16(sqlStatement, 1));

            // TODO: Take parent id into consideration here
            _theoryChapters.Append(make<implementation::TheoryChapter>(id, title));
        }

        if (resultCode != SQLITE_DONE)
        {
            std::string s = "Ошибка базы данных: ";
            s += sqlite3_errmsg(db);
            co_await ShowErrorContentDialog(L"Ошибка базы данных", to_hstring(s));
        }

        sqlite3_finalize(sqlStatement);
        sqlite3_close(db);

        NavView().SelectedItem(TheoryChapters().GetAt(0));
    }

    IAsyncAction TheoryPage::ShowErrorContentDialog(hstring const& title, hstring const& content) const
    {
        const ContentDialog dialog;
        dialog.XamlRoot(this->XamlRoot());
        dialog.Style(unbox_value<Microsoft::UI::Xaml::Style>(Application::Current().Resources().Lookup(box_value(L"DefaultContentDialogStyle"))));
        dialog.Title(box_value(title));
        dialog.Content(box_value(content));
        dialog.CloseButtonText(L"Ок");

        co_await dialog.ShowAsync();
    }
}
