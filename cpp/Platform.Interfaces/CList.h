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
        return requires(const Self self, std::size_t index, std::tuple<TItems...> items, decltype(std::get<0>(items)) item, std::ranges::iterator_t<const Self> const_iterator) {
          { self.size() } -> std::integral;
          { self.empty() } -> std::same_as<bool>;
          { self[index] } -> std::convertible_to<decltype(item)>;
          
          requires std::ranges::forward_range<const Self>;
        };
      }
      if constexpr (sizeof...(TItems) == 0) {
        return requires(const Self self, std::size_t index, typename Enumerable<const Self>::Item generic_item, typename Enumerable<const Self>::Iter const_iterator) {
          { self.size() } -> std::integral;
          { self.empty() } -> std::same_as<bool>;
          { self[index] } -> std::convertible_to<decltype(generic_item)>;
          
          requires std::ranges::forward_range<const Self>;
        };
      }

      return false;
    }
  }  // namespace Internal

  template <typename TSelf, typename... TItems>
  concept CList = CArray<TSelf> && Internal::CListHelpFunction<TSelf, TItems...>();

  template <typename TSelf, typename... TItems>
  concept CReadonlyList = CArray<TSelf> && Internal::CReadonlyListHelpFunction<TSelf, TItems...>();

  template <CList TSelf>
  struct List : Enumerable<TSelf> {};

  template <CReadonlyList TSelf>
  struct ReadonlyList : Enumerable<TSelf> {};
}  // namespace Platform::Interfaces