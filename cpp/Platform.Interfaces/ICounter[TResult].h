namespace Platform::Interfaces
{
    template <typename Self, typename TResult, typename TArgument = Internal::nil>
    concept ICounter = requires(Self self, TArgument argument)
    {
        requires
            requires()
            {
                requires std::same_as<TArgument, Internal::nil>;
                { self.Count() } -> std::same_as<TResult>;
            }
            ||
            requires()
            {
                { self.Count(argument) } -> std::same_as<TResult>;
            };
    };
}
