namespace Platform::Interfaces
{
    template <typename Self, typename TProvider, typename... TArgument>
    concept IProvider = sizeof...(TArgument) <= 1 &&
    requires(Self self, TArgument... argument)
    {
        { self.Get(argument...) } -> std::same_as<TProvider>;
    };
}
