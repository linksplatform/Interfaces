#pragma once

#include <concepts>

namespace Platform::Interfaces
{
    template <typename TSelf, typename TResult, typename... TArgument>
    concept CCounter = sizeof...(TArgument) <= 1 &&
    requires(TSelf self, TArgument... argument)
    {
        { self.Count(argument...) } -> std::same_as<TResult>;
    };
}
