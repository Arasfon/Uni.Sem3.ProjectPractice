#include "pch.h"

#include "App.xaml.h"

#include "Debug.h"
#include "MainWindow.xaml.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace FiberOpticCommsCourseware;
using namespace FiberOpticCommsCourseware::implementation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    });
#endif
}

/// <summary>
/// Invoked when the application is launched.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const&)
{
    int argc;
    const auto argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (argc > 1 && CSTR_EQUAL == CompareStringOrdinal(argv[1], -1, L"--debug-exam-answers", -1, false))
        Debug::ShowExamAnswers = true;

    LocalFree(argv);

    _window = make<MainWindow>();
    _window.Activate();
}
