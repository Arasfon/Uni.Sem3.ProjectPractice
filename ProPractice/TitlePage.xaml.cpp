#include "pch.h"
#include "TitlePage.xaml.h"
#if __has_include("TitlePage.g.cpp")
#include "TitlePage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::ProPractice::implementation
{
    TitlePage::TitlePage()
    {
        InitializeComponent();
    }
}
