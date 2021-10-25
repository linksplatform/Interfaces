#pragma once

#include <concepts>
#include <type_traits>
#include <tuple>
#include <ranges>
#include <utility>
#include <cstdio>

#include "CEnumerable.h"

namespace Platform::Interfaces
{
    namespace Internal
    {
        template<typename TSelf, typename... Items>
        consteval bool CArrayHelpFunction()
        {
            constexpr bool member_indexator = requires(TSelf self, std::size_t index)
            {
                { self[index] } -> std::same_as<typename Enumerable<TSelf>::ItemReference>;
            };

            if constexpr (sizeof...(Items) == 1)
            {
                using SelfItem = typename Enumerable<TSelf>::Item;
                using RequiredItem = std::remove_reference_t<decltype(std::get<0>(std::declval<std::tuple<Items...>>()))>;

                return member_indexator && std::ranges::random_access_range<TSelf> && std::same_as<SelfItem, RequiredItem>;
            }
            if constexpr (sizeof...(Items) == 0)
            {
                return member_indexator &&  std::ranges::random_access_range<TSelf>;
            }

            return false;
        }
    }

    template<typename TSelf, typename... Item>
    concept CArray = CEnumerable<TSelf> && Internal::CArrayHelpFunction<TSelf, Item...>();

    template<CArray TSelf>
    struct Array : Enumerable<TSelf> {};
}
