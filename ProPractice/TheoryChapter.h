#pragma once
#include "TheoryChapter.g.h"

namespace winrt::ProPractice::implementation
{
    struct TheoryChapter : TheoryChapterT<TheoryChapter>
    {
        TheoryChapter() = delete;

        TheoryChapter(int64_t id, hstring const& title);

        WINRT_READONLY_PROPERTY(int64_t, Id);
        WINRT_READONLY_PROPERTY(hstring, Title);

        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IObservableVector<winrt::ProPractice::TheoryChapter>, Children, winrt::single_threaded_observable_vector<ProPractice::TheoryChapter>());
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct TheoryChapter : TheoryChapterT<TheoryChapter, implementation::TheoryChapter>
    {
    };
}
