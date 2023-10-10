// Uses names and parts of WinUIEx library (https://github.com/dotMorten/WinUIEx).
// Copyright (c) 2021 Morten Nielsen. All rights reserved. Licensed under MIT License.

#include "pch.h"

#include "WinUIEx.h"
#include <Microsoft.UI.Xaml.Window.h>

HWND WinUIEx::GetWindowHandle(winrt::Microsoft::UI::Xaml::Window const& window)
{
    const auto windowNative { window.as<IWindowNative>() };
    HWND hwnd { nullptr };
    windowNative->get_WindowHandle(&hwnd);
    return hwnd;
}

void WinUIEx::CenterOnScreen(winrt::Microsoft::UI::Xaml::Window const& window)
{
    const HWND hwnd = GetWindowHandle(window);
    const HMONITOR hwndDesktop = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info { 0 };
    info.cbSize = 40;
    GetMonitorInfo(hwndDesktop, &info);
    RECT windowRect { 0 };
    GetWindowRect(hwnd, &windowRect);
    const long w = windowRect.right - windowRect.left;
    const long h = windowRect.bottom - windowRect.top;
    const long cx = (info.rcMonitor.left + info.rcMonitor.right) / 2;
    const long cy = (info.rcMonitor.bottom + info.rcMonitor.top) / 2;
    const long left = cx - (w / 2);
    const long top = cy - (h / 2);
    const bool result = SetWindowPos(hwnd, nullptr, left, top, w, h, 0);
    if (!result)
        winrt::throw_hresult(HRESULT_FROM_WIN32(GetLastError()));
}
