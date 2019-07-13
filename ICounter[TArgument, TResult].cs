namespace Platform.Interfaces
{
    public interface ICounter<TArgument, TResult>
    {
        TResult Count(TArgument argument);
    }
}
