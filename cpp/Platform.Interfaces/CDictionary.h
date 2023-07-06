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
  }  // namespace Internal

  template <typename TSelf, typename... TArgs>
  concept CDictionary = CEnumerable<TSelf> && Internal::CDictionaryHelpFunction<TSelf, TArgs...>();

  template <CDictionary TSelf>
  struct Dictionary : Enumerable<TSelf> {
    using base = Enumerable<TSelf>;
    using Key = decltype(std::get<0>(std::declval<base::Item>()));
    using Value = decltype(std::get<1>(std::declval<base::Item>()));
  };
}  // namespace Platform::Interfaces
