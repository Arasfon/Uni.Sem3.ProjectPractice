#include "pch.h"
#include "ExamStartPage.xaml.h"
#if __has_include("ExamStartPage.g.cpp")
#include "ExamStartPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::ProPractice::implementation
{
    ExamStartPage::ExamStartPage()
    {
        InitializeComponent();
    }

    void ExamStartPage::OnNavigatedTo(Navigation::NavigationEventArgs const& e)
    {
        _examController = unbox_value<ExamController>(e.Parameter());
    }

    void ExamStartPage::StartButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        _examController.CallControl(ExamControlAction::Start);
    }
}
