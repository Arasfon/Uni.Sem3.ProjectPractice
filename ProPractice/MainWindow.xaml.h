#pragma once

#include "MainWindow.g.h"

namespace winrt::ProPractice::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
        void NavView_SelectionChanged(IInspectable const& sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& args);
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
