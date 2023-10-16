// Uses names and parts of WinUIEx library (https://github.com/dotMorten/WinUIEx).
// Copyright (c) 2021 Morten Nielsen. All rights reserved. Licensed under MIT License.

#include "pch.h"

#include "WinUIEx.h"
#include <Microsoft.UI.Xaml.Window.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Windows::Foundation;

HWND WinUIEx::GetWindowHandle(Window const& window)
{
    const auto windowNative { window.as<IWindowNative>() };
    HWND hwnd { nullptr };
    windowNative->get_WindowHandle(&hwnd);
    return hwnd;
}

void WinUIEx::CenterOnScreen(Window const& window)
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
        throw_hresult(HRESULT_FROM_WIN32(GetLastError()));
}

void WinUIEx::SetIcon(Window const& window, hstring const& iconPath)
{
    HANDLE image = LoadImage(nullptr, iconPath.c_str(), IMAGE_ICON, 44, 44, LR_LOADFROMFILE);
    const HWND windowHwnd = GetWindowHandle(window);
    SendMessage(windowHwnd, WM_SETICON, reinterpret_cast<WPARAM>(nullptr), reinterpret_cast<LPARAM>(image));
}

IAsyncAction WinUIEx::ShowSimpleContentDialog(UIElement const& uiElement, hstring const& title, hstring const& content)
{
    const Controls::ContentDialog dialog;
    dialog.XamlRoot(uiElement.XamlRoot());
    dialog.Style(unbox_value<Style>(Application::Current().Resources().Lookup(box_value(L"DefaultContentDialogStyle"))));
    dialog.Title(box_value(title));
    dialog.Content(box_value(content));
    dialog.CloseButtonText(L"Ок");

    co_await dialog.ShowAsync();
}