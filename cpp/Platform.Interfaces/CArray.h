namespace Platform::Interfaces
{
    namespace Internal
    {
        template<typename Self, typename... Items>
        consteval bool CArrayHelpFunction()
        {
            constexpr bool member_indexator = requires(Self self, std::size_t index)
            {
                { self[index] } -> std::same_as<typename Enumerable<Self>::ItemReference>;
            };

            if constexpr (sizeof...(Items) == 1)
            {
                using SelfItem = typename Enumerable<Self>::Item;
                using RequiredItem = std::remove_reference_t<decltype(std::get<0>(std::declval<std::tuple<Items...>>()))>;

                return member_indexator && std::ranges::random_access_range<Self> && std::same_as<SelfItem, RequiredItem>;
            }
            if constexpr (sizeof...(Items) == 0)
            {
                return member_indexator &&  std::ranges::random_access_range<Self>;
            }

            return false;
        }
    }

    template<typename Self, typename... Item>
    concept CArray = CEnumerable<Self> && Internal::CArrayHelpFunction<Self, Item...>();

    template<CArray Self>
    struct Array : Enumerable<Self> {};
}
