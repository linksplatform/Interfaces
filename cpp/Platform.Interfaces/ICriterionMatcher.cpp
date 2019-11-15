namespace Platform::Interfaces
{
    template <typename TArgument> class ICriterionMatcher
    {
    public:
        virtual bool IsMatched(TArgument argument) = 0;
    };
}
