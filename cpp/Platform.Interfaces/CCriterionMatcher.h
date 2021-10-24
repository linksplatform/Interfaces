namespace Platform::Interfaces
{
    template <typename Self, typename TArgument>
    concept CCriterionMatcher = requires(Self self, TArgument argument)
    {
        { self.IsMatched(argument) } -> std::same_as<bool>;
    };
}
