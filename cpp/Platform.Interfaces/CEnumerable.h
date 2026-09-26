#pragma once

#include <ranges>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires a type that can be iterated as a range.</para>
  /// <para>Требует тип, который можно перебирать как диапазон.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <typename TSelf>
  concept CEnumerable = std::ranges::range<TSelf>;

  /// <summary>
  /// <para>Exposes the item, reference and iterator types of an enumerable.</para>
  /// <para>Предоставляет типы элемента, ссылки и итератора перечисляемого объекта.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <CEnumerable TSelf>
  struct Enumerable {
    /// <summary>
    /// <para>The item value type.</para>
    /// <para>Тип значения элемента.</para>
    /// </summary>
    using Item = std::ranges::range_value_t<TSelf>;
    /// <summary>
    /// <para>The item reference type.</para>
    /// <para>Тип ссылки на элемент.</para>
    /// </summary>
    using ItemReference = std::ranges::range_reference_t<TSelf>;
    /// <summary>
    /// <para>The iterator type.</para>
    /// <para>Тип итератора.</para>
    /// </summary>
    using Iter = std::ranges::iterator_t<TSelf>;
  };
}  // namespace Platform::Interfaces
