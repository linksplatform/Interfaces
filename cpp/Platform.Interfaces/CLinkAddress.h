#pragma once

#include <concepts>
#include <type_traits>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires an unsigned integral link address.</para>
  /// <para>Требует беззнаковый целочисленный адрес связи.</para>
  /// </summary>
  /// <typeparam name="T">
  /// <para>The type checked as a link address.</para>
  /// <para>Тип, проверяемый как адрес связи.</para>
  /// </typeparam>
  template <typename T>
  concept CLinkAddress = std::is_integral<T>::value && std::is_unsigned<T>::value;
}
