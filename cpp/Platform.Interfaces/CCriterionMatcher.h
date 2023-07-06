#pragma once

#include <concepts>

namespace Platform::Interfaces {
  template <typename TSelf, typename TArgument>
  concept CCriterionMatcher = requires(TSelf self, TArgument argument) {
    { self.IsMatched(argument) } -> std::same_as<bool>;
  };
}  // namespace Platform::Interfaces
