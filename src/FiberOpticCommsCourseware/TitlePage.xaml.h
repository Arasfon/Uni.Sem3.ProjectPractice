#pragma once

#include "TitlePage.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct TitlePage : TitlePageT<TitlePage>
    {
        TitlePage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        winrt::fire_and_forget CheckData();
        int64_t GetDataVersion();

    private:
        inline static const std::wstring DataCurrentVersionUri = L"https://functions.yandexcloud.net/your-function-here";
        FiberOpticCommsCourseware::MainWindow _mainWindow = nullptr;
        inline static bool _dataChecked = false;
    };
}

namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct TitlePage : TitlePageT<TitlePage, implementation::TitlePage>
    {
    };
}
