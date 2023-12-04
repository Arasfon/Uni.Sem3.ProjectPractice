#include "pch.h"
#include "TheoryChapter.h"
#if __has_include("TheoryChapter.g.cpp")
#include "TheoryChapter.g.cpp"
#endif

namespace winrt::ProPractice::implementation
{
    TheoryChapter::TheoryChapter(const int64_t id, hstring const& title)
        : _Id { id }, _Title { title }
    {

    }
}
