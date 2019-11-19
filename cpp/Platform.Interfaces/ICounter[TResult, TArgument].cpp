namespace Platform::Interfaces
{
    template <typename TResult, typename TArgument> class ICounter<TResult, TArgument>
    {
    public:
        virtual TResult Count(TArgument argument) = 0;
    };
}
