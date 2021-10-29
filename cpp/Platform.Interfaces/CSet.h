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
        template<typename TRawSelf, typename... TItems>
        consteval bool CSetHelpFunction()
        {
            using Self = std::remove_const_t<TRawSelf>;

            if constexpr (sizeof...(TItems) == 1)
            {
                return requires
                (
                    Self self,
                    std::tuple<TItems...> items,
                    decltype(std::get<0>(items)) item
                )
                {
                    { self.find(item) } -> std::same_as<std::ranges::iterator_t<Self>>;
                    { self.insert(item) };
                    { self.erase(item) };
                    { self.contains(item) } -> std::same_as<bool>;
                    { self.empty() } -> std::same_as<bool>;
                    { self.size() } -> std::integral;
                    { self.clear() };

                    requires std::ranges::forward_range<Self>;
                };
            }
            if constexpr (sizeof...(TItems) == 0)
            {
                return requires
                (
                    Self self,
                    typename Enumerable<Self>::Item generic_item
                )
                {
                    { self.find(generic_item) } -> std::same_as<std::ranges::iterator_t<Self>>;
                    { self.insert(generic_item) };
                    { self.erase(generic_item) };
                    { self.contains(generic_item) } -> std::same_as<bool>;
                    { self.empty() } -> std::same_as<bool>;
                    { self.size() } -> std::integral;
                    { self.clear() };

                    requires std::ranges::forward_range<Self>;
                };
            }

            return false;
        }

    }

    template<typename TSelf, typename... TItem>
    concept CSet = CEnumerable<TSelf> && Internal::CSetHelpFunction<TSelf, TItem...>();

    template<CSet TSelf>
    struct Set : Enumerable<TSelf> {};
}
