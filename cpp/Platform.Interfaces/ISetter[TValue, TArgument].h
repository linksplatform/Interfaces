namespace Platform::Interfaces
{
    template <typename Self, typename TValue, typename... TArgument>
    concept ISetter = sizeof...(TArgument) <= 1 &&
    requires(Self self, TArgument... argument, TValue value)
    {
        { self.Set(argument..., value) } -> std::same_as<void>;
    };
}
