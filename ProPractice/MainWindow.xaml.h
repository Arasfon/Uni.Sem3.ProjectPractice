#pragma once

#include "MainWindow.g.h"

namespace winrt::ProPractice::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        bool AreNavigationMenuItemsEnabled();
        void AreNavigationMenuItemsEnabled(bool value);

        void NavView_SelectionChanged(IInspectable const& sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& args);

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        bool _areNavigationMenuItemsEnabled = true;
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> _propertyChanged;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
