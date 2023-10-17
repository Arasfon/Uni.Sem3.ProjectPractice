#pragma once
#include "TheoryChapter.g.h"

namespace winrt::ProPractice::implementation
{
    struct TheoryChapter : TheoryChapterT<TheoryChapter>
    {
        TheoryChapter() = delete;

        TheoryChapter(int64_t id, hstring const& title);
        int64_t Id() const;
        void Id(int64_t value);
        hstring Title();
        void Title(hstring const& value);
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::ProPractice::TheoryChapter> Children();
        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        hstring _title;
        int64_t _id;
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::ProPractice::TheoryChapter> _children;
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> _propertyChanged;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct TheoryChapter : TheoryChapterT<TheoryChapter, implementation::TheoryChapter>
    {
    };
}
