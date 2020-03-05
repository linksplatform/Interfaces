namespace Platform::Interfaces
{
    template <typename...> class ICriterionMatcher; template <typename TArgument> class ICriterionMatcher<TArgument>
    {
    public:
        virtual bool IsMatched(TArgument argument) = 0;
    };
}
