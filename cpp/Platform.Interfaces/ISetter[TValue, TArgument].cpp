namespace Platform::Interfaces
{
    template <typename TValue, typename TArgument> class ISetter
    {
    public:
        virtual void Set(TArgument argument, TValue value) = 0;
    };
}
