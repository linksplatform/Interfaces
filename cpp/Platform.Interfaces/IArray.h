namespace Platform::Interfaces
{
    template<typename Self, typename... Item>
    concept IArray =
        IEnumerable<Self> &&
        sizeof...(Item) <= 1 &&
    requires(std::tuple<Item...> items)
    {
        requires
            requires
            {
                requires sizeof...(Item) == 1;

                requires std::same_as<std::ranges::range_value_t<Self>, decltype(std::get<0>(items))>;
                requires std::ranges::random_access_range<Self>;
            }
            ||
            requires
            {
                requires sizeof...(Item) == 0;

                requires std::ranges::random_access_range<Self>;
            };
    };

    template<IArray Self>
    struct Array : Enumerable<Self> {};
}
