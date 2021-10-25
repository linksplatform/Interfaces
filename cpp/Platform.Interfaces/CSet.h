#pragma once

#include <type_traits>
#include <tuple>
#include <ranges>
#include <concepts>

#include "CEnumerable.h"

namespace Platform::Interfaces
{
    namespace Internal
    {
        template<typename RawSelf, typename... Items>
        consteval bool CSetHelpFunction()
        {
            using TSelf = std::remove_const_t<RawSelf>;

            if constexpr (sizeof...(Items) == 1)
            {
                return requires
                (
                    TSelf self,
                    std::tuple<Items...> items,
                    decltype(std::get<0>(items)) item
                )
                {
                    { self.find(item) } -> std::same_as<std::ranges::iterator_t<TSelf>>;
                    { self.insert(item) };
                    { self.erase(item) };
                    { self.contains(item) } -> std::same_as<bool>;
                    { self.empty() } -> std::same_as<bool>;
                    { self.size() } -> std::integral;
                    { self.clear() };

                    requires std::ranges::forward_range<TSelf>;
                };
            }
            if constexpr (sizeof...(Items) == 0)
            {
                return requires
                (
                    TSelf self,
                    typename Enumerable<TSelf>::Item generic_item
                )
                {
                    { self.find(generic_item) } -> std::same_as<std::ranges::iterator_t<TSelf>>;
                    { self.insert(generic_item) };
                    { self.erase(generic_item) };
                    { self.contains(generic_item) } -> std::same_as<bool>;
                    { self.empty() } -> std::same_as<bool>;
                    { self.size() } -> std::integral;
                    { self.clear() };

                    requires std::ranges::forward_range<TSelf>;
                };
            }

            return false;
        }

    }

    template<typename TSelf, typename... Item>
    concept CSet = CEnumerable<TSelf> && Internal::CSetHelpFunction<TSelf, Item...>();

    template<CSet TSelf>
    struct Set : Enumerable<TSelf> {};
}
