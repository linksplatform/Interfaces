#pragma once

#include <type_traits>
#include <cstddef>
#include <tuple>
#include <ranges>
#include <concepts>

#include "CEnumerable.h"
#include "CArray.h"

namespace Platform::Interfaces
{
    namespace Internal
    {
        template<typename TRawSelf, typename... TItems>
        consteval bool CListHelpFunction()
        {
            using Self = std::remove_const_t<TRawSelf>;

            if constexpr (sizeof...(TItems) == 1)
            {
                return requires
                (
                    Self self,
                    std::size_t index,
                    std::tuple<TItems...> items,
                    decltype(std::get<0>(items)) item,
                    std::ranges::iterator_t<const Self> const_iterator
                )
                {
                    { self.push_back(item) };
                    { self.insert(const_iterator, item) };
                    { self.erase(const_iterator) };
                    { self.size() } -> std::integral;
                    { self.clear() };
                };
            }
            if constexpr (sizeof...(TItems) == 0)
            {
                return requires
                (
                    Self self,
                    std::size_t index,
                    typename Enumerable<Self>::Item generic_item,
                    typename Enumerable<const Self>::Iter const_iterator
                )
                {
                    { self.push_back(generic_item) };
                    { self.insert(const_iterator, generic_item) };
                    { self.erase(const_iterator) };
                    { self.size() } -> std::integral;
                    { self.clear() };
                };
            }

            return false;
        }
    }

    template<typename TSelf, typename... TItems>
    concept CList = CArray<TSelf> && Internal::CListHelpFunction<TSelf, TItems...>();

    template<CList TSelf>
    struct List : Enumerable<TSelf> {};
}
