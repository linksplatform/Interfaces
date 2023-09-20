#pragma once

#include <type_traits>

namespace Platform::Interfaces {

template <typename T>
struct CLinkAddress {
    static constexpr bool value = std::is_integral<T>::value && std::is_unsigned<T>::value;
};

}  // namespace Platform::Interfaces
