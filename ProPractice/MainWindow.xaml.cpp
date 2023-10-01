#include "pch.h"

#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::ProPractice::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        ExtendsContentIntoTitleBar(true);
        SetTitleBar(AppTitleBar());
    }

    void MainWindow::NavView_SelectionChanged(IInspectable const& sender, NavigationViewSelectionChangedEventArgs const& args)
    {
        const auto selectedItem = unbox_value<IFrameworkElement>(args.SelectedItem());
        const TypeName typeName { L"ProPractice." + unbox_value<hstring>(selectedItem.Tag()) + L"Page", TypeKind::Metadata};
        ContentFrame().Navigate(typeName);
    }
}
