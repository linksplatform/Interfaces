#pragma once

#include <concepts>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires a counter returning the specified result, with at most one argument.</para>
  /// <para>Требует счётчик, возвращающий заданный результат и принимающий не более одного аргумента.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TResult">
  /// <para>The result type.</para>
  /// <para>Тип результата.</para>
  /// </typeparam>
  /// <typeparam name="TArgument">
  /// <para>Optional argument type.</para>
  /// <para>Необязательный тип аргумента.</para>
  /// </typeparam>
  template <typename TSelf, typename TResult, typename... TArgument>
  concept CCounter = sizeof...(TArgument) <= 1 && requires(TSelf self, TArgument... argument) {
    { self.Count(argument...) } -> std::same_as<TResult>;
  };
}  // namespace Platform::Interfaces
