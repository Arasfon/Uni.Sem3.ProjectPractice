#pragma once

#include "TheoryPage.g.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::Web::WebView2::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::ProPractice::implementation
{
    struct TheoryPage : TheoryPageT<TheoryPage>
    {
        TheoryPage();
        IObservableVector<ProPractice::TheoryChapter> TheoryChapters();
        void OnNavigatedFrom(NavigationEventArgs const& e);
        IAsyncAction OnWebViewInitialized(WebView2 const& sender, CoreWebView2InitializedEventArgs const& e);
        void NavView_SelectionChanged(IInspectable const& sender, NavigationViewSelectionChangedEventArgs const& args);
        IAsyncAction OnWebViewWebResourceRequested(CoreWebView2 const& sender, CoreWebView2WebResourceRequestedEventArgs const& e);
        IAsyncAction LoadChapters();
        IAsyncAction ShowErrorContentDialog(const hstring& title, const hstring& content) const;

    private:
        IObservableVector<ProPractice::TheoryChapter> _theoryChapters;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct TheoryPage : TheoryPageT<TheoryPage, implementation::TheoryPage>
    {
    };
}
