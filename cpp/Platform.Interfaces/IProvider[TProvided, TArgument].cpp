namespace Platform::Interfaces
{
    template <typename TProvided, typename TArgument> class IProvider<TProvided, TArgument>
    {
    public:
        virtual TProvided Get(TArgument argument) = 0;
    };
}
