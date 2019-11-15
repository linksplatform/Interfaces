namespace Platform::Interfaces
{
    interface ISetter<in TValue, in TArgument>
    {
        void Set(TArgument argument, TValue value);
    }
}
