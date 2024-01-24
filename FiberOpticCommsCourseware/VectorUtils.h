#pragma once

#include <random>

class VectorUtils
{
public:
    template <typename T>
    static void ShuffleVector(winrt::Windows::Foundation::Collections::IVector<T> const& vector)
    {
        std::mt19937 randomEngine(_randomDevice());

        for (unsigned int i = vector.Size() - 1; i > 0; --i)
        {
            std::uniform_int_distribution<> distribution(0, i);
            SwapVectorItems(vector, i, distribution(randomEngine));
        }
    }

    template <typename T>
    static void SwapVectorItems(winrt::Windows::Foundation::Collections::IVector<T> const& vector, unsigned int firstIndex, unsigned int secondIndex)
    {
        auto item = vector.GetAt(firstIndex);
        vector.SetAt(firstIndex, vector.GetAt(secondIndex));
        vector.SetAt(secondIndex, item);
    }

private:
    inline static std::random_device _randomDevice{};
};
