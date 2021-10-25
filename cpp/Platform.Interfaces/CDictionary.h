#pragma once

#include <concepts>
#include <type_traits>
#include <tuple>
#include <utility>
#include <iterator>
#include <ranges>

#include "CEnumerable.h"

namespace Platform::Interfaces
{
    namespace Internal
    {
        template<typename RawSelf, typename... Args>
        consteval bool CDictionaryHelpFunction()
        {
            using TSelf = std::remove_const_t<RawSelf>;

            using GenericKey = std::remove_reference_t<decltype(std::get<0>(std::declval<typename Enumerable<TSelf>::Item>()))>;
            using GenericValue = std::remove_reference_t<decltype(std::get<1>(std::declval<typename Enumerable<TSelf>::Item>()))>;

            if constexpr (sizeof...(Args) == 0)
            {
                return requires
                (
                    TSelf self,
                    GenericKey generic_key,
                    GenericValue generic_value
                )
                {
                    { self[generic_key] } -> std::same_as<GenericValue&>;
                    { self.find(generic_key) } -> std::forward_iterator;
                    { self.contains(generic_key) } -> std::same_as<bool>;
                    { self.insert({generic_key, generic_value}) };
                    { self.empty() } -> std::same_as<bool>;
                    { self.size() } -> std::integral;
                    { self.clear() };

                    requires std::ranges::forward_range<TSelf>;
                };
            }
            if constexpr (sizeof...(Args) == 1)
            {
                return requires
                (
                    TSelf self,
                    std::tuple<Args...> args,

                    decltype(std::get<0>(args)) key,
                    decltype(std::declval<Enumerable<TSelf>::Item>().second) generic_value
                )
                {
                    { self[key] } -> std::same_as<GenericValue&>;
                    { self.find(key) } -> std::forward_iterator;
                    { self.contains(key) } -> std::same_as<bool>;
                    { self.insert({key, generic_value}) };
                    { self.empty() } -> std::same_as<bool>;
                    { self.size() } -> std::integral;
                    { self.clear() };

                    requires std::ranges::forward_range<TSelf>;
                };
            }
            if constexpr (sizeof...(Args) == 2)
            {
                return requires
                (
                    TSelf self,
                    std::tuple<Args...> args,

                    decltype(std::get<0>(args)) key,
                    decltype(std::get<1>(args)) value
                )
                {
                    { self[key] } -> std::same_as<GenericValue&>;
                    { self.find(key) } -> std::forward_iterator;
                    { self.contains(key) } -> std::same_as<bool>;
                    { self.insert({key, value}) };
                    { self.empty() } -> std::same_as<bool>;
                    { self.size() } -> std::integral;
                    { self.clear() };

                    requires std::ranges::forward_range<TSelf>;
                };
            }

            return false;
        }
    }

    template<typename TSelf, typename... Args>
    concept CDictionary = CEnumerable<TSelf> && Internal::CDictionaryHelpFunction<TSelf, Args...>();

    template<CDictionary TSelf>
    struct Dictionary : Enumerable<TSelf>
    {
    private:
        using base = Enumerable<TSelf>;

    public:
        using Key = decltype(std::get<0>(std::declval<base::Item>()));
        using Value = decltype(std::get<1>(std::declval<base::Item>()));
    };
}
