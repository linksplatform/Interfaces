namespace Platform.Interfaces
{
    public interface IProvider<TProvided, TArgument>
    {
        TProvided Get(TArgument argument);
    }
}
