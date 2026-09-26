#pragma once

#include <concepts>
#include <ranges>
#include <tuple>
#include <type_traits>

#include "CEnumerable.h"

namespace Platform::Interfaces {
  namespace Internal {
    template <typename TRawSelf, typename... TItems>
    consteval bool CSetHelpFunction() {
      using Self = std::remove_const_t<TRawSelf>;

      if constexpr (sizeof...(TItems) == 1) {
        return requires(Self self, std::tuple<TItems...> items, decltype(std::get<0>(items)) item) {
          { self.find(item) } -> std::same_as<std::ranges::iterator_t<Self>>;
          {self.insert(item)};
          {self.erase(item)};
          { self.contains(item) } -> std::same_as<bool>;
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;
          {self.clear()};

          requires std::ranges::forward_range<Self>;
        };
      }
      if constexpr (sizeof...(TItems) == 0) {
        return requires(Self self, typename Enumerable<Self>::Item generic_item) {
          { self.find(generic_item) } -> std::same_as<std::ranges::iterator_t<Self>>;
          {self.insert(generic_item)};
          {self.erase(generic_item)};
          { self.contains(generic_item) } -> std::same_as<bool>;
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;
          {self.clear()};

          requires std::ranges::forward_range<Self>;
        };
      }

      return false;
    }

    template <typename TRawSelf, typename... TItems>
    consteval bool CReadonlySetHelpFunction() {
      using Self = TRawSelf;

      if constexpr (sizeof...(TItems) == 1) {
        return requires(const Self& self, std::tuple<TItems...> items, decltype(std::get<0>(items)) item) {
          { self.find(item) } -> std::same_as<std::ranges::iterator_t<const Self>>;
          { self.contains(item) } -> std::same_as<bool>;
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;

          requires std::ranges::forward_range<const Self>;
        };
      }
      if constexpr (sizeof...(TItems) == 0) {
        return requires(const Self& self, typename Enumerable<const Self>::Item generic_item) {
          { self.find(generic_item) } -> std::same_as<std::ranges::iterator_t<const Self>>;
          { self.contains(generic_item) } -> std::same_as<bool>;
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;

          requires std::ranges::forward_range<const Self>;
        };
      }

      return false;
    }

  }  // namespace Internal

  /// <summary>
  /// <para>Requires a mutable enumerable set with lookup, insertion and removal operations.</para>
  /// <para>Требует изменяемое перечисляемое множество с поиском, вставкой и удалением.</para>
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
  concept CSet = CEnumerable<TSelf> && Internal::CSetHelpFunction<TSelf, TItems...>();

  /// <summary>
  /// <para>Requires a read-only enumerable set with lookup operations.</para>
  /// <para>Требует доступное только для чтения перечисляемое множество с поиском.</para>
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
  concept CReadonlySet = CEnumerable<TSelf> && Internal::CReadonlySetHelpFunction<TSelf, TItems...>();

  /// <summary>
  /// <para>Exposes the item and iterator types of a mutable set.</para>
  /// <para>Предоставляет типы элемента и итератора изменяемого множества.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <CSet TSelf>
  struct Set : Enumerable<TSelf> {};

  /// <summary>
  /// <para>Exposes the item and iterator types of a read-only set.</para>
  /// <para>Предоставляет типы элемента и итератора множества только для чтения.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <CReadonlySet TSelf>
  struct ReadonlySet : Enumerable<TSelf> {};
}  // namespace Platform::Interfaces
