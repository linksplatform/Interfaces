namespace Platform::Interfaces
{
    interface ISetter<in TValue>
    {
        void Set(TValue value);
    }
}
