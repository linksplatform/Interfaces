#pragma once

#include <concepts>

namespace Platform::Interfaces {
  template <typename TSelf, typename TValue, typename... TArgument>
  concept CSetter = sizeof...(TArgument) <= 1 && requires(TSelf self, TArgument... argument, TValue value) {
    { self.Set(argument..., value) } -> std::same_as<void>;
  };
}  // namespace Platform::Interfaces
