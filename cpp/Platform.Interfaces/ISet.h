namespace Platform::Interfaces
{
    namespace Internal
    {
        template<typename RawSelf, typename... Items>
        consteval bool ISetHelpFunction()
        {
            using Self = std::remove_const_t<RawSelf>;

            if constexpr (sizeof...(Items) == 1)
            {
                return requires
                (
                    Self self,
                    std::tuple<Items...> items,
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
            if constexpr (sizeof...(Items) == 0)
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

    template<typename Self, typename... Item>
    concept ISet = IEnumerable<Self> && Internal::ISetHelpFunction<Self, Item...>();

    template<ISet Self>
    struct Set : Enumerable<Self> {};
}
