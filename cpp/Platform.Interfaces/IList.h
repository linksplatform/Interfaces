#ifndef ILIST
#define ILIST

namespace Platform::Interfaces
{
    template<typename Self, typename... Item>
    concept IList =
        IArray<Self> &&
        sizeof...(Item) <= 1 &&
    requires
        (
            Self self,
            std::tuple<Item...> items,

            std::size_t index,
            decltype(std::get<0>(items)) item,

            std::ranges::range_value_t<Self> generic_item,

            std::ranges::iterator_t<const Self> const_iterator
        )
    {
        requires
        requires
        {
            requires sizeof...(Item) == 1;

            { self[index] } -> std::same_as<typename Enumerable<Self>::ItemReference>;
            { self.push_back(item) };
            { self.insert(const_iterator, item) };
            { self.erase(const_iterator) };
        }
        ||
        requires
        {
            requires sizeof...(Item) == 0;

            { self[index] } -> std::same_as<typename Enumerable<Self>::ItemReference>;
            { self.push_back(generic_item) };
            { self.insert(const_iterator, generic_item) };
            { self.erase(const_iterator) };
        };

        { self.size() } -> std::integral;
        { self.clear() };
    };

    template<IList Self>
    struct List : Enumerable<Self> {};
}

#endif //ILIST
