namespace Platform::Interfaces
{
    template <typename Self, typename TProvider, typename TArgument = Internal::nil>
    concept IProvider = requires(Self self, TArgument argument)
    {
        requires
        requires
        {
            requires std::same_as<TArgument, Internal::nil>;
            { self.Get() } -> std::same_as<TProvider>;
        }
        ||
        requires
        {
            { self.Get(argument) } -> std::same_as<TProvider>;
        };
    };
}
