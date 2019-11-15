namespace Platform::Interfaces
{
    template <typename TProvided, typename TArgument> class IProvider
    {
    public:
        virtual TProvided Get(TArgument argument) = 0;
    }
}
