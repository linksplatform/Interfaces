#pragma once

#include <concepts>
#include <type_traits>

namespace Platform::Interfaces {
  template <typename T>
  concept CLinkAddress = std::is_integral<T>::value && std::is_unsigned<T>::value;
}
