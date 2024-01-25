#pragma once

#include "TheoryPage.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct TheoryPage : TheoryPageT<TheoryPage>
    {
        TheoryPage();

        WINRT_READONLY_PROPERTY(Windows::Foundation::Collections::IObservableVector<FiberOpticCommsCourseware::TheoryChapter>, TheoryChapters, winrt::single_threaded_observable_vector<FiberOpticCommsCourseware::TheoryChapter>());

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);
        void OnNavigatedFrom(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        Windows::Foundation::IAsyncAction OnWebViewInitialized(Microsoft::UI::Xaml::Controls::WebView2 const& sender, Microsoft::UI::Xaml::Controls::CoreWebView2InitializedEventArgs const& e);
        void NavView_SelectionChanged(IInspectable const& sender, Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& e);
        Windows::Foundation::IAsyncAction OnWebViewWebResourceRequested(Microsoft::Web::WebView2::Core::CoreWebView2 const& sender, Microsoft::Web::WebView2::Core::CoreWebView2WebResourceRequestedEventArgs const& e);

        Windows::Foundation::IAsyncAction LoadChapters();

    private:
        std::unordered_map<int64_t, std::vector<unsigned int>> _chapterPaths;
    };
}

namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct TheoryPage : TheoryPageT<TheoryPage, implementation::TheoryPage>
    {
    };
}
