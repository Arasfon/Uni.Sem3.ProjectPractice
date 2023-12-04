#pragma once

// Uses parts of WinUIEx library (https://github.com/dotMorten/WinUIEx).
// Copyright (c) 2021 Morten Nielsen. All rights reserved. Licensed under MIT License.

// ReSharper disable once CppInconsistentNaming

class WinUIEx
{
public:
    static HWND GetWindowHandle(winrt::Microsoft::UI::Xaml::Window const& window);
    static void CenterOnScreen(winrt::Microsoft::UI::Xaml::Window const& window);
    static void SetIcon(winrt::Microsoft::UI::Xaml::Window const& window, winrt::hstring const& iconPath);
    [[nodiscard]] static winrt::Windows::Foundation::IAsyncAction ShowSimpleContentDialog(winrt::Microsoft::UI::Xaml::UIElement const& uiElement, winrt::hstring const& title, winrt::hstring const& content);
};
