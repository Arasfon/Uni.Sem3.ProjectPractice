#pragma once

#include "TheoryPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct TheoryPage : TheoryPageT<TheoryPage>
    {
        TheoryPage();
        Windows::Foundation::Collections::IObservableVector<ProPractice::TheoryChapter> TheoryChapters();
        void OnNavigatedFrom(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);
        Windows::Foundation::IAsyncAction OnWebViewInitialized(Microsoft::UI::Xaml::Controls::WebView2 const& sender, Microsoft::UI::Xaml::Controls::CoreWebView2InitializedEventArgs const& e);
        void NavView_SelectionChanged(IInspectable const& sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& e);
        Windows::Foundation::IAsyncAction OnWebViewWebResourceRequested(Microsoft::Web::WebView2::Core::CoreWebView2 const& sender, Microsoft::Web::WebView2::Core::CoreWebView2WebResourceRequestedEventArgs const& e);
        Windows::Foundation::IAsyncAction LoadChapters();
        [[nodiscard]] Windows::Foundation::IAsyncAction ShowErrorContentDialog(hstring const& title, hstring const& content) const;

    private:
        Windows::Foundation::Collections::IObservableVector<ProPractice::TheoryChapter> _theoryChapters;
        std::unordered_map<int64_t, std::vector<unsigned int>> _chapterPaths;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct TheoryPage : TheoryPageT<TheoryPage, implementation::TheoryPage>
    {
    };
}
