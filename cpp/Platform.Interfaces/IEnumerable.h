#ifndef IENUMERABLE
#define IENUMERABLE

namespace Platform::Interfaces
{
    template<typename Self>
    concept IEnumerable = std::ranges::range<Self>;

    template<IEnumerable Self>
    struct Enumerable
    {
        using Item = std::ranges::range_value_t<Self>;
        using ItemReference = std::ranges::range_reference_t<Self>;
        using Iter = std::ranges::iterator_t<Self>;
    };
}

#endif //IENUMERABLE
