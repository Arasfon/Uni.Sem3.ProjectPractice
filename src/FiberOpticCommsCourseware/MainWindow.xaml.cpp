#include "pch.h"

#include "MainWindow.xaml.h"

#include "WinUIEx.h"

#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::FiberOpticCommsCourseware::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        ExtendsContentIntoTitleBar(true);
        SetTitleBar(AppTitleBar());
        WinUIEx::SetIcon(*this, L"Assets/Square150x150Logo.scale-200.ico");
        WinUIEx::CenterOnScreen(*this);
    }

    void MainWindow::NavView_SelectionChanged(IInspectable const&, NavigationViewSelectionChangedEventArgs const& e)
    {
        const auto selectedItem = unbox_value<FrameworkElement>(e.SelectedItem());
        const TypeName pageTypeName { L"FiberOpticCommsCourseware." + unbox_value<hstring>(selectedItem.Tag()) + L"Page", TypeKind::Metadata };
        // ReSharper disable once CppExpressionWithoutSideEffects
        ContentFrame().Navigate(pageTypeName, *this);
    }
}
