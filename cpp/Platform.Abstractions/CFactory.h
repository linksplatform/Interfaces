#pragma once

#include <concepts>

namespace Platform::Abstractions {
  template <typename TSelf, typename TProduct>
  concept CFactory = requires(TSelf self) {
    { self.Create() } -> std::same_as<TProduct>;
  };
}  // namespace Platform::Abstractions
