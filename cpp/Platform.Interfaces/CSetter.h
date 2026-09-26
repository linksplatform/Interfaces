#pragma once

#include <concepts>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires a setter that accepts a value and at most one additional argument.</para>
  /// <para>Требует установщик, принимающий значение и не более одного дополнительного аргумента.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TValue">
  /// <para>The value type.</para>
  /// <para>Тип значения.</para>
  /// </typeparam>
  /// <typeparam name="TArgument">
  /// <para>Optional argument type.</para>
  /// <para>Необязательный тип аргумента.</para>
  /// </typeparam>
  template <typename TSelf, typename TValue, typename... TArgument>
  concept CSetter = sizeof...(TArgument) <= 1 && requires(TSelf self, TArgument... argument, TValue value) {
    { self.Set(argument..., value) } -> std::same_as<void>;
  };
}  // namespace Platform::Interfaces
