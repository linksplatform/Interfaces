namespace Platform::Interfaces
{
    interface IProvider<out TProvided, in TArgument>
    {
        TProvided Get(TArgument argument);
    }
}
