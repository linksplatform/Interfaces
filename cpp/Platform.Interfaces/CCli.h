#pragma once

#include <concepts>
#include <string>
#include <vector>

namespace Platform::Interfaces {
  template <typename TSelf>
  concept CCli = requires(TSelf self, const std::vector<std::string>& args) {
    { self.Run(args) } -> std::same_as<int>;
  };
}  // namespace Platform::Interfaces