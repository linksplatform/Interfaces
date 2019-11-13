namespace Platform::Interfaces
{
    interface ICounter<out TResult>
    {
        TResult Count();
    }
}
