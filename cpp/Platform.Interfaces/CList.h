#pragma once

#include "CEnumerable.h"
#include "CArray.h"

namespace Platform::Interfaces
{
    namespace Internal
    {
        template<typename RawSelf, typename... Item>
        consteval bool CListHelpFunction()
        {
            using TSelf = std::remove_const_t<RawSelf>;

            if constexpr (sizeof...(Item) == 1)
            {
                return requires
                (
                    TSelf self,
                    std::size_t index,
                    std::tuple<Item...> items,
                    decltype(std::get<0>(items)) item,
                    std::ranges::iterator_t<const TSelf> const_iterator
                )
                {
                    { self.push_back(item) };
                    { self.insert(const_iterator, item) };
                    { self.erase(const_iterator) };
                    { self.size() } -> std::integral;
                    { self.clear() };
                };
            }
            if constexpr (sizeof...(Item) == 0)
            {
                return requires
                (
                    TSelf self,
                    std::size_t index,
                    typename Enumerable<TSelf>::Item generic_item,
                    typename Enumerable<const TSelf>::Iter const_iterator
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

    template<typename TSelf, typename... Item>
    concept CList = CArray<TSelf> && Internal::CListHelpFunction<TSelf, Item...>();

    template<CList TSelf>
    struct List : Enumerable<TSelf> {};
}
