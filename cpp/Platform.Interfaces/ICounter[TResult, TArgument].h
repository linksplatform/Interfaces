namespace Platform::Interfaces
{
    template <typename Self, typename TResult, typename... TArgument>
    concept ICounter = sizeof...(TArgument) <= 1 &&
                       requires(Self self, TArgument... argument)
{
{ self.Count(argument...) } -> std::same_as<TResult>;
};
}
