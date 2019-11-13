namespace Platform::Interfaces
{
    interface IProvider<out TProvided>
    {
        TProvided Get();
    }
}
