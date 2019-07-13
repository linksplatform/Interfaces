namespace Platform.Interfaces
{
    public interface ICreteriaMatcher<TArgument>
    {
        bool IsMatched(TArgument argument);
    }
}
