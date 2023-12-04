#pragma once

#include "MainWindow.g.h"

namespace winrt::ProPractice::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        WINRT_OBSERVABLE_PROPERTY(bool, AreNavigationMenuItemsEnabled, _PropertyChangedHandlers, true);
        WINRT_OBSERVABLE_PROPERTY(bool, IsDataCurrent, _PropertyChangedHandlers, false);

        void NavView_SelectionChanged(IInspectable const& sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& e);

        WINRT_CALLBACK(PropertyChanged, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler);
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
