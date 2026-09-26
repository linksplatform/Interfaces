#pragma once

#include <concepts>
#include <cstddef>
#include <ranges>
#include <tuple>
#include <type_traits>

#include "CArray.h"
#include "CEnumerable.h"

namespace Platform::Interfaces {
  namespace Internal {
    template <typename TRawSelf, typename... TItems>
    consteval bool CListHelpFunction() {
      using Self = std::remove_const_t<TRawSelf>;

      if constexpr (sizeof...(TItems) == 1) {
        return requires(Self self, std::size_t index, std::tuple<TItems...> items, decltype(std::get<0>(items)) item, std::ranges::iterator_t<const Self> const_iterator) {
          {self.push_back(item)};
          {self.insert(const_iterator, item)};
          {self.erase(const_iterator)};
          { self.size() } -> std::integral;
          {self.clear()};
        };
      }
      if constexpr (sizeof...(TItems) == 0) {
        return requires(Self self, std::size_t index, typename Enumerable<Self>::Item generic_item, typename Enumerable<const Self>::Iter const_iterator) {
          {self.push_back(generic_item)};
          {self.insert(const_iterator, generic_item)};
          {self.erase(const_iterator)};
          { self.size() } -> std::integral;
          {self.clear()};
        };
      }

      return false;
    }

    template <typename TRawSelf, typename... TItems>
    consteval bool CReadonlyListHelpFunction() {
      using Self = TRawSelf;

      if constexpr (sizeof...(TItems) == 1) {
        using RequiredItem = std::remove_reference_t<decltype(std::get<0>(std::declval<std::tuple<TItems...>>()))>;

        return requires(const Self& self, std::size_t index) {
          { self.size() } -> std::integral;
          { self.empty() } -> std::same_as<bool>;
          { self[index] } -> std::convertible_to<RequiredItem>;

          requires std::ranges::forward_range<const Self>;
        };
      }
      if constexpr (sizeof...(TItems) == 0) {
        using GenericItem = typename Enumerable<const Self>::Item;

        return requires(const Self& self, std::size_t index) {
          { self.size() } -> std::integral;
          { self.empty() } -> std::same_as<bool>;
          { self[index] } -> std::convertible_to<GenericItem>;

          requires std::ranges::forward_range<const Self>;
        };
      }

      return false;
    }
  }  // namespace Internal

  /// <summary>
  /// <para>Requires an indexed mutable list with insertion and removal operations.</para>
  /// <para>Требует индексируемый изменяемый список с операциями вставки и удаления.</para>
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
  concept CList = CArray<TSelf> && Internal::CListHelpFunction<TSelf, TItems...>();

  /// <summary>
  /// <para>Requires an indexed read-only list with size and empty-state queries.</para>
  /// <para>Требует индексируемый список только для чтения с запросами размера и пустого состояния.</para>
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
  concept CReadonlyList = CArray<TSelf> && Internal::CReadonlyListHelpFunction<TSelf, TItems...>();

  /// <summary>
  /// <para>Exposes the item and iterator types of a mutable list.</para>
  /// <para>Предоставляет типы элемента и итератора изменяемого списка.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <CList TSelf>
  struct List : Enumerable<TSelf> {};

  /// <summary>
  /// <para>Exposes the item and iterator types of a read-only list.</para>
  /// <para>Предоставляет типы элемента и итератора списка только для чтения.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <CReadonlyList TSelf>
  struct ReadonlyList : Enumerable<TSelf> {};
}  // namespace Platform::Interfaces
