#pragma once

#include <concepts>

namespace Platform::Interfaces {
  template <typename TSelf, typename TProvider, typename... TArgument>
  concept CProvider = sizeof...(TArgument) <= 1 && requires(TSelf self, TArgument... argument) {
    { self.Get(argument...) } -> std::same_as<TProvider>;
  };
}  // namespace Platform::Interfaces
