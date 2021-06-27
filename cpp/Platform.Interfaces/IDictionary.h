namespace Platform::Interfaces
{
    namespace Internal
    {
        template<typename RawSelf, typename... Args>
        consteval bool IDictionaryHelpFunction()
        {
            using Self = std::remove_const_t<RawSelf>;

            using GenericKey = std::remove_reference_t<decltype(std::get<0>(std::declval<typename Enumerable<Self>::Item>()))>;
            using GenericValue = std::remove_reference_t<decltype(std::get<1>(std::declval<typename Enumerable<Self>::Item>()))>;

            if constexpr (sizeof...(Args) == 0)
            {
                return requires
                (
                    Self self,
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

                    requires std::ranges::bidirectional_range<Self>;
                };
            }
            if constexpr (sizeof...(Args) == 1)
            {
                return requires
                (
                    Self self,
                    std::tuple<Args...> args,

                    decltype(std::get<0>(args)) key,
                    decltype(std::declval<Enumerable<Self>::Item>().second) generic_value
                )
                {
                    { self[key] } -> std::same_as<GenericValue&>;
                    { self.find(key) } -> std::forward_iterator;
                    { self.contains(key) } -> std::same_as<bool>;
                    { self.insert({key, generic_value}) };
                    { self.empty() } -> std::same_as<bool>;
                    { self.size() } -> std::integral;
                    { self.clear() };

                    requires std::ranges::bidirectional_range<Self>;
                };
            }
            if constexpr (sizeof...(Args) == 2)
            {
                return requires
                (
                    Self self,
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

                    requires std::ranges::bidirectional_range<Self>;
                };
            }

            return false;
        }
    }

    template<typename Self, typename... Args>
    concept IDictionary = IEnumerable<Self> && Internal::IDictionaryHelpFunction<Self, Args...>();

    template<IDictionary Self>
    struct Dictionary : Enumerable<Self>
    {
    private:
        using base = Enumerable<Self>;

    public:
        using Key = decltype(std::declval<base::Item>().first);
        using Value = decltype(std::declval<base::Item>().second);
    };
}
