namespace Platform::Interfaces
{
    template <typename TResult, typename TArgument> class ICounter
    {
    public:
        virtual TResult Count(TArgument argument) = 0;
    };
}
