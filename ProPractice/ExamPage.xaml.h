#pragma once

#include "ExamPage.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamPage : ExamPageT<ExamPage>
    {
        ExamPage();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void myButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct ExamPage : ExamPageT<ExamPage, implementation::ExamPage>
    {
    };
}
