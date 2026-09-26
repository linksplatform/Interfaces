#pragma once

#include <concepts>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires a provider of the specified type with at most one argument.</para>
  /// <para>Требует поставщика заданного типа, принимающего не более одного аргумента.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TProvider">
  /// <para>The provided type.</para>
  /// <para>Предоставляемый тип.</para>
  /// </typeparam>
  /// <typeparam name="TArgument">
  /// <para>Optional argument type.</para>
  /// <para>Необязательный тип аргумента.</para>
  /// </typeparam>
  template <typename TSelf, typename TProvider, typename... TArgument>
  concept CProvider = sizeof...(TArgument) <= 1 && requires(TSelf self, TArgument... argument) {
    { self.Get(argument...) } -> std::same_as<TProvider>;
  };
}  // namespace Platform::Interfaces
