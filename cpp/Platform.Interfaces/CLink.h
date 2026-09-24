#pragma once

#include <concepts>
#include <type_traits>

namespace Platform::Interfaces {
  template <typename TSelf>
  concept CLink = requires(TSelf self) {
    { self.empty() } -> std::same_as<bool>;
    typename TSelf::value_type;
    { self.begin } -> std::same_as<typename TSelf::value_type&>;
    { self.end } -> std::same_as<typename TSelf::value_type&>;
  };

  template <CLink TSelf>
  struct Link {
    using value_type = typename TSelf::value_type;
  };
}  // namespace Platform::Interfaces