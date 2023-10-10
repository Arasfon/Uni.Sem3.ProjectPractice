#pragma once

// Uses names and parts of WinUIEx library (https://github.com/dotMorten/WinUIEx).
// Copyright (c) 2021 Morten Nielsen. All rights reserved. Licensed under MIT License.

// ReSharper disable once CppInconsistentNaming
class WinUIEx
{
public:
    static HWND GetWindowHandle(winrt::Microsoft::UI::Xaml::Window const& window);
    static void CenterOnScreen(winrt::Microsoft::UI::Xaml::Window const& window);
};
