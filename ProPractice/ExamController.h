#pragma once

#include "ExamController.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamController : ExamControllerT<ExamController>
    {
        ExamController() = default;

        WINRT_PROPERTY(uint32_t, CurrentQuestion, 0);
        WINRT_READONLY_PROPERTY(winrt::Windows::Foundation::Collections::IVector<winrt::ProPractice::IExamQuestion>, Questions, single_threaded_vector<IExamQuestion>());

        WINRT_CALLBACK(OnControlAction, winrt::Windows::Foundation::EventHandler<winrt::ProPractice::ExamControlAction>);

        void CallControl(ExamControlAction action);
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamController : ExamControllerT<ExamController, implementation::ExamController>
    {
    };
}
