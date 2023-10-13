#pragma once

#include "ExamController.g.h"

namespace winrt::ProPractice::implementation
{
    struct ExamController : ExamControllerT<ExamController>
    {
        ExamController();

        int32_t CurrentQuestion();
        void CurrentQuestion(int32_t const& value);

        winrt::Windows::Foundation::Collections::IVector<winrt::ProPractice::ExamQuestion> Questions();

        winrt::event_token OnControlAction(winrt::Windows::Foundation::EventHandler<winrt::ProPractice::ExamControlAction> const& handler);
        void OnControlAction(winrt::event_token const& token) noexcept;

        void CallControl(const ExamControlAction action);

    private:
        int32_t _currentQuestion = 0;
        winrt::Windows::Foundation::Collections::IVector<ExamQuestion> _questions = nullptr;
        winrt::event<winrt::Windows::Foundation::EventHandler<winrt::ProPractice::ExamControlAction>> _onControlActionEvent;
    };
}
namespace winrt::ProPractice::factory_implementation
{
    struct ExamController : ExamControllerT<ExamController, implementation::ExamController>
    {
    };
}
