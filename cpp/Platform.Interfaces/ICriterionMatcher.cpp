namespace Platform::Interfaces
{
    interface ICriterionMatcher<in TArgument>
    {
        bool IsMatched(TArgument argument);
    }
}
