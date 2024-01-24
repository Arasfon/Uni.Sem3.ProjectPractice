#include "pch.h"

#include "ExamController.h"
#if __has_include("ExamController.g.cpp")
#include "ExamController.g.cpp"
#endif

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::FiberOpticCommsCourseware::implementation
{
    void ExamController::CallControl(const ExamControlAction action)
    {
        _OnControlActionHandlers(*this, action);
    }
}
