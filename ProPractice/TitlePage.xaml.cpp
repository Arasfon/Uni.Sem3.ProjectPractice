#include "pch.h"
#include "TitlePage.xaml.h"
#if __has_include("TitlePage.g.cpp")
#include "TitlePage.g.cpp"
#endif

#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Storage.Streams.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Windows::Data::Json;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage;

namespace winrt::ProPractice::implementation
{
    TitlePage::TitlePage()
    {
        InitializeComponent();
    }

    void TitlePage::OnNavigatedTo(NavigationEventArgs const& e)
    {
        _mainWindow = unbox_value<MainWindow>(e.Parameter());

        if(!_dataChecked)
        {
            DataLoadingStackPanel().Visibility(Visibility::Visible);
            CheckData();
        }
    }

    fire_and_forget TitlePage::CheckData()
    {
        _dataChecked = true;

        _mainWindow.AreNavigationMenuItemsEnabled(false);

        Uri uri { DataCurrentVersionUri };
        const HttpClient httpClient;

        hstring versionDataRaw;

        try
        {
            versionDataRaw = co_await httpClient.GetStringAsync(uri);
        }
        catch(hresult_error const&)
        {
            DataLoadingStackPanel().Visibility(Visibility::Collapsed);
            _mainWindow.IsDataCurrent(false);
            _mainWindow.AreNavigationMenuItemsEnabled(true);
            co_return;
        }

        JsonObject versionData;
        if (!JsonObject::TryParse(versionDataRaw, versionData))
        {
            DataLoadingStackPanel().Visibility(Visibility::Collapsed);
            _mainWindow.IsDataCurrent(false);
            _mainWindow.AreNavigationMenuItemsEnabled(true);
            co_return;
        }

        const int64_t dataVersion = GetDataVersion();

        if (dataVersion < versionData.GetNamedNumber(L"currentVersion"))
        {
            DataLoadingTextBlock().Text(L"Загружаем обновлённые данные...");

            uri = Uri { versionData.GetNamedString(L"downloadUrl") };

            IBuffer buffer;
            try
            {
                buffer = co_await httpClient.GetBufferAsync(uri);
            }
            catch(hresult_error const&)
            {
                DataLoadingStackPanel().Visibility(Visibility::Collapsed);
                _mainWindow.IsDataCurrent(false);
                _mainWindow.AreNavigationMenuItemsEnabled(true);
                co_return;
            }

            SetLastError(ERROR_SUCCESS);
            wchar_t* currentFolderPathBuffer = nullptr;
            DWORD currentFolderPathBufferLength = MAX_PATH / 2;
            DWORD currentFolderPathLength = 0;
            do
            {
                currentFolderPathBufferLength *= 2;
                delete[] currentFolderPathBuffer;
                currentFolderPathBuffer = new wchar_t[currentFolderPathBufferLength];
                currentFolderPathLength = GetModuleFileName(nullptr, currentFolderPathBuffer, currentFolderPathBufferLength - 1);
            }
            while (currentFolderPathLength == currentFolderPathBufferLength - 1 && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

            const StorageFolder storageFolder = co_await (co_await StorageFile::GetFileFromPathAsync(currentFolderPathBuffer)).GetParentAsync();

            const auto databaseFile { co_await storageFolder.CreateFileAsync(L"data.db", CreationCollisionOption::ReplaceExisting) };
            co_await FileIO::WriteBufferAsync(databaseFile, buffer);

            delete[] currentFolderPathBuffer;
        }

        DataLoadingStackPanel().Visibility(Visibility::Collapsed);
        _mainWindow.IsDataCurrent(true);
        _mainWindow.AreNavigationMenuItemsEnabled(true);
    }

    int64_t TitlePage::GetDataVersion()
    {
        sqlite3* db;
        int resultCode = sqlite3_open_v2("data.db", &db, SQLITE_OPEN_READONLY, nullptr);
        if (resultCode != SQLITE_OK)
        {
            sqlite3_close(db);
            return -1;
        }

        sqlite3_stmt* sqlStatement;
        resultCode = sqlite3_prepare_v2(db, "SELECT value FROM parameters WHERE name = 'data_version';", -1, &sqlStatement, nullptr);
        if (resultCode != SQLITE_OK)
        {
            sqlite3_close(db);
            return -1;
        }

        int64_t version = -1;

        while ((resultCode = sqlite3_step(sqlStatement)) == SQLITE_ROW)
        {
            const auto* value = static_cast<const wchar_t*>(sqlite3_column_text16(sqlStatement, 0));
            version = std::stoll(value);
        }

        if (resultCode != SQLITE_DONE)
        {
            sqlite3_finalize(sqlStatement);
            sqlite3_close(db);
            return -1;
        }

        sqlite3_finalize(sqlStatement);
        sqlite3_close(db);

        return version;
    }
}
