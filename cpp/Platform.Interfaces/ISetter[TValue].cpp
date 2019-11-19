namespace Platform::Interfaces
{
    template <typename TValue> class ISetter<TValue>
    {
    public:
        virtual void Set(TValue value) = 0;
    };
}
