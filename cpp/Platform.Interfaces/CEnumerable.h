#pragma once

namespace Platform::Interfaces
{
    template<typename TSelf>
    concept CEnumerable = std::ranges::range<TSelf>;

    template<CEnumerable TSelf>
    struct Enumerable
    {
        using Item = std::ranges::range_value_t<TSelf>;
        using ItemReference = std::ranges::range_reference_t<TSelf>;
        using Iter = std::ranges::iterator_t<TSelf>;
    };
}
