namespace Platform::Interfaces
{
    namespace Internal
    {
        template<typename Self, typename... Item>
        consteval bool IListHelpFunction()
        {
            if constexpr (sizeof...(Item) == 1)
            {
                return requires
                (
                    Self self,
                    std::size_t index,
                    std::tuple<Item...> items,
                    decltype(std::get<0>(items)) item,
                    std::ranges::iterator_t<const Self> const_iterator
                )
                {
                    { self[index] } -> std::same_as<typename Enumerable<Self>::ItemReference>;
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
                    Self self,
                    std::size_t index,
                    typename Enumerable<Self>::Item generic_item,
                    typename Enumerable<const Self>::Iter const_iterator
                )
                {
                    { self[index] } -> std::same_as<typename Enumerable<Self>::ItemReference>;
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

    template<typename Self, typename... Item>
    concept IList = IArray<Self> && Internal::IListHelpFunction<Self, Item...>();

    template<IList Self>
    struct List : Enumerable<Self> {};
}
