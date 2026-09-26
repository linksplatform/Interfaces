#pragma once

#include <concepts>

namespace Platform::Interfaces {
  template <typename TSelf, typename TCandidate>
  concept CMatcher = requires(TSelf self, TCandidate candidate) {
    { self.IsMatched(candidate) } -> std::same_as<bool>;
  };
}  // namespace Platform::Interfaces
