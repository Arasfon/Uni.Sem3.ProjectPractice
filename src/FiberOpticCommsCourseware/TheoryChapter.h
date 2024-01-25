#pragma once
#include "TheoryChapter.g.h"

namespace winrt::FiberOpticCommsCourseware::implementation
{
    struct TheoryChapter : TheoryChapterT<TheoryChapter>
    {
        TheoryChapter() = delete;

        TheoryChapter(int64_t id, hstring const& title);

        WINRT_READONLY_PROPERTY(int64_t, Id);
        WINRT_READONLY_PROPERTY(hstring, Title);

        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IObservableVector<winrt::FiberOpticCommsCourseware::TheoryChapter>, Children, winrt::single_threaded_observable_vector<FiberOpticCommsCourseware::TheoryChapter>());
    };
}

namespace winrt::FiberOpticCommsCourseware::factory_implementation
{
    struct TheoryChapter : TheoryChapterT<TheoryChapter, implementation::TheoryChapter>
    {
    };
}
