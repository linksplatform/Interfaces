namespace Platform.Interfaces
{
    public interface IProvider<TProvided>
    {
        TProvided Get();
    }
}
