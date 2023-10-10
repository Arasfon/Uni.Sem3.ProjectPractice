#pragma once

#include "TitlePage.g.h"

namespace winrt::ProPractice::implementation
{
    struct TitlePage : TitlePageT<TitlePage>
    {
        TitlePage();
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct TitlePage : TitlePageT<TitlePage, implementation::TitlePage>
    {
    };
}
