#pragma once

#include <concepts>
#include <cstdio>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

#include "CEnumerable.h"

namespace Platform::Interfaces {
  namespace Internal {
    template <typename TSelf, typename... TItems>
    consteval bool CArrayHelpFunction() {
      constexpr bool member_indexator = requires(TSelf self, std::size_t index) { {self[index]} /*-> std::same_as<typename Enumerable<TSelf>::ItemReference>*/; };

      if constexpr (sizeof...(TItems) == 1) {
        using SelfItem = typename Enumerable<TSelf>::Item;
        using RequiredItem = std::remove_reference_t<decltype(std::get<0>(std::declval<std::tuple<TItems...>>()))>;

        return member_indexator && std::ranges::random_access_range<TSelf> && std::same_as<SelfItem, RequiredItem>;
      }
      if constexpr (sizeof...(TItems) == 0) {
        return member_indexator && std::ranges::random_access_range<TSelf>;
      }

      return false;
    }
  }  // namespace Internal

  /// <summary>
  /// <para>Requires an enumerable with indexed random access and, optionally, a specified item type.</para>
  /// <para>Требует перечисляемую коллекцию с произвольным доступом по индексу и, при необходимости, заданным типом элемента.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TItems">
  /// <para>Optional item types.</para>
  /// <para>Необязательные типы элементов.</para>
  /// </typeparam>
  template <typename TSelf, typename... TItems>
  concept CArray = CEnumerable<TSelf> && Internal::CArrayHelpFunction<TSelf, TItems...>();

  /// <summary>
  /// <para>Exposes the item and iterator types of an array.</para>
  /// <para>Предоставляет типы элементов и итератора массива.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <CArray TSelf>
  struct Array : Enumerable<TSelf> {};
}  // namespace Platform::Interfaces
