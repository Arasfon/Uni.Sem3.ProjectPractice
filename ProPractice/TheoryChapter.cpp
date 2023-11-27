#include "pch.h"
#include "TheoryChapter.h"
#if __has_include("TheoryChapter.g.cpp")
#include "TheoryChapter.g.cpp"
#endif

namespace winrt::ProPractice::implementation
{
    TheoryChapter::TheoryChapter(const int64_t id, hstring const& title)
        : _id { id }, _title { title }
    {
        _children = winrt::single_threaded_observable_vector<ProPractice::TheoryChapter>();
    }

    int64_t TheoryChapter::Id() const
    {
        return _id;
    }

    void TheoryChapter::Id(const int64_t value)
    {
        if (_id != value)
        {
            _id = value;
            _propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs { L"Id" });
        }
    }

    hstring TheoryChapter::Title()
    {
        return _title;
    }

    void TheoryChapter::Title(hstring const& value)
    {
        if (_title != value)
        {
            _title = value;
            _propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs { L"Title" });
        }
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::ProPractice::TheoryChapter> TheoryChapter::Children()
    {
        return _children;
    }

    winrt::event_token TheoryChapter::PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return _propertyChanged.add(handler);
    }

    void TheoryChapter::PropertyChanged(winrt::event_token const& token) noexcept
    {
        _propertyChanged.remove(token);
    }
}
