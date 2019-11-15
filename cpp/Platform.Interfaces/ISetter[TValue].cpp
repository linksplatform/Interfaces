namespace Platform::Interfaces
{
    template <typename TValue> class ISetter
    {
    public:
        virtual void Set(TValue value) = 0;
    };
}
