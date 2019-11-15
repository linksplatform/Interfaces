namespace Platform::Interfaces
{
    interface ICounter<out TResult, in TArgument>
    {
        TResult Count(TArgument argument);
    }
}
