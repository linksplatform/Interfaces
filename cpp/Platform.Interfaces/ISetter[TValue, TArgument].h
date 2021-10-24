namespace Platform::Interfaces
{
    template <typename ...> class ISetter;
    template <typename TValue, typename TArgument> class ISetter<TValue, TArgument>
    {
    public:
        virtual void Set(TArgument argument, TValue value) = 0;
    };
}
