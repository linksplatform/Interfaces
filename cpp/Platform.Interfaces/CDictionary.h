#pragma once

#include <concepts>
#include <iterator>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

#include "CEnumerable.h"

namespace Platform::Interfaces {
  namespace Internal {
    template <typename TRawSelf, typename... TArgs>
    consteval bool CDictionaryHelpFunction() {
      using Self = std::remove_const_t<TRawSelf>;

      using GenericKey = std::remove_reference_t<decltype(std::get<0>(std::declval<typename Enumerable<Self>::Item>()))>;
      using GenericValue = std::remove_reference_t<decltype(std::get<1>(std::declval<typename Enumerable<Self>::Item>()))>;

      if constexpr (sizeof...(TArgs) == 0) {
        return requires(Self self, GenericKey generic_key, GenericValue generic_value) {
          { self[generic_key] } -> std::same_as<GenericValue&>;
          { self.find(generic_key) } -> std::forward_iterator;
          { self.contains(generic_key) } -> std::same_as<bool>;
          {self.insert({generic_key, generic_value})};
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;
          {self.clear()};

          requires std::ranges::forward_range<Self>;
        };
      }
      if constexpr (sizeof...(TArgs) == 1) {
        return requires(Self self, std::tuple<TArgs...> args,

                        decltype(std::get<0>(args)) key, decltype(std::declval<Enumerable<Self>::Item>().second) generic_value) {
          { self[key] } -> std::same_as<GenericValue&>;
          { self.find(key) } -> std::forward_iterator;
          { self.contains(key) } -> std::same_as<bool>;
          {self.insert({key, generic_value})};
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;
          {self.clear()};

          requires std::ranges::forward_range<Self>;
        };
      }
      if constexpr (sizeof...(TArgs) == 2) {
        return requires(Self self, std::tuple<TArgs...> args,

                        decltype(std::get<0>(args)) key, decltype(std::get<1>(args)) value) {
          { self[key] } -> std::same_as<GenericValue&>;
          { self.find(key) } -> std::forward_iterator;
          { self.contains(key) } -> std::same_as<bool>;
          {self.insert({key, value})};
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;
          {self.clear()};

          requires std::ranges::forward_range<Self>;
        };
      }

      return false;
    }

    template <typename TRawSelf, typename... TArgs>
    consteval bool CReadonlyDictionaryHelpFunction() {
      using Self = TRawSelf;

      using GenericKey = std::remove_reference_t<decltype(std::get<0>(std::declval<typename Enumerable<Self>::Item>()))>;

      if constexpr (sizeof...(TArgs) == 0) {
        return requires(const Self& self, GenericKey generic_key) {
          { self.find(generic_key) } -> std::forward_iterator;
          { self.contains(generic_key) } -> std::same_as<bool>;
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;

          requires std::ranges::forward_range<const Self>;
        };
      }
      if constexpr (sizeof...(TArgs) == 1) {
        return requires(const Self& self, std::tuple<TArgs...> args,
                        decltype(std::get<0>(args)) key) {
          { self.find(key) } -> std::forward_iterator;
          { self.contains(key) } -> std::same_as<bool>;
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;

          requires std::ranges::forward_range<const Self>;
        };
      }
      if constexpr (sizeof...(TArgs) == 2) {
        return requires(const Self& self, std::tuple<TArgs...> args,
                        decltype(std::get<0>(args)) key) {
          { self.find(key) } -> std::forward_iterator;
          { self.contains(key) } -> std::same_as<bool>;
          { self.empty() } -> std::same_as<bool>;
          { self.size() } -> std::integral;

          requires std::ranges::forward_range<const Self>;
        };
      }

      return false;
    }
  }  // namespace Internal

  /// <summary>
  /// <para>Requires a mutable enumerable dictionary with lookup, insertion and removal operations.</para>
  /// <para>Требует изменяемый перечисляемый словарь с поиском, вставкой и удалением.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TArgs">
  /// <para>Optional key and value types.</para>
  /// <para>Необязательные типы ключа и значения.</para>
  /// </typeparam>
  template <typename TSelf, typename... TArgs>
  concept CDictionary = CEnumerable<TSelf> && Internal::CDictionaryHelpFunction<TSelf, TArgs...>();

  /// <summary>
  /// <para>Requires a read-only enumerable dictionary with lookup operations.</para>
  /// <para>Требует доступный только для чтения перечисляемый словарь с операциями поиска.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TArgs">
  /// <para>Optional key and value types.</para>
  /// <para>Необязательные типы ключа и значения.</para>
  /// </typeparam>
  template <typename TSelf, typename... TArgs>
  concept CReadonlyDictionary = CEnumerable<TSelf> && Internal::CReadonlyDictionaryHelpFunction<TSelf, TArgs...>();

  /// <summary>
  /// <para>Exposes the item, key and value types of a mutable dictionary.</para>
  /// <para>Предоставляет типы элемента, ключа и значения изменяемого словаря.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <CDictionary TSelf>
  struct Dictionary : Enumerable<TSelf> {
    /// <summary>
    /// <para>The enumerable base type.</para>
    /// <para>Базовый перечисляемый тип.</para>
    /// </summary>
    using base = Enumerable<TSelf>;
    /// <summary>
    /// <para>The key type.</para>
    /// <para>Тип ключа.</para>
    /// </summary>
    using Key = decltype(std::get<0>(std::declval<typename base::Item>()));
    /// <summary>
    /// <para>The value type.</para>
    /// <para>Тип значения.</para>
    /// </summary>
    using Value = decltype(std::get<1>(std::declval<typename base::Item>()));
  };

  /// <summary>
  /// <para>Exposes the item, key and value types of a read-only dictionary.</para>
  /// <para>Предоставляет типы элемента, ключа и значения словаря, доступного только для чтения.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <CReadonlyDictionary TSelf>
  struct ReadonlyDictionary : Enumerable<TSelf> {
    /// <summary>
    /// <para>The enumerable base type.</para>
    /// <para>Базовый перечисляемый тип.</para>
    /// </summary>
    using base = Enumerable<TSelf>;
    /// <summary>
    /// <para>The key type.</para>
    /// <para>Тип ключа.</para>
    /// </summary>
    using Key = decltype(std::get<0>(std::declval<typename base::Item>()));
    /// <summary>
    /// <para>The value type.</para>
    /// <para>Тип значения.</para>
    /// </summary>
    using Value = decltype(std::get<1>(std::declval<typename base::Item>()));
  };
}  // namespace Platform::Interfaces
