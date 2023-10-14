#pragma once

#include "ExamControlHostPage.g.h"

#include <random>

namespace winrt::ProPractice::implementation
{
    struct ExamControlHostPage : ExamControlHostPageT<ExamControlHostPage>
    {
        ExamControlHostPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void OnControlAction(IInspectable const& sender, ExamControlAction action);

        Windows::Foundation::IAsyncAction LoadQuestions();

        template <typename T>
        void ShuffleVector(Windows::Foundation::Collections::IVector<T> const& vector);
        template <typename T>
        void SwapVectorItems(Windows::Foundation::Collections::IVector<T> const& vector, unsigned int firstIndex, unsigned int secondIndex);

        [[nodiscard]] Windows::Foundation::IAsyncAction ShowErrorContentDialog(hstring const& title, hstring const& content) const;

    private:
        ProPractice::MainWindow _mainWindow = nullptr;
        ProPractice::ExamController _examController = nullptr;
        inline static std::random_device _randomDevice;
    };
}

namespace winrt::ProPractice::factory_implementation
{
    struct ExamControlHostPage : ExamControlHostPageT<ExamControlHostPage, implementation::ExamControlHostPage>
    {
    };
}
