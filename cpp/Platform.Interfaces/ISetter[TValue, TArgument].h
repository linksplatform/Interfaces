namespace Platform::Interfaces
{
    template <typename Self, typename TValue, typename TArgument = Internal::nil>
    concept ISetter = requires(Self self, TArgument argument, TValue value)
    {
        requires
            requires
            {
                requires std::same_as<TArgument, Internal::nil>;
                { self.Set(value) } -> std::same_as<void>;
            }
            ||
            requires
            {
                { self.Set(argument, value) } -> std::same_as<void>;
            };
    };
}
