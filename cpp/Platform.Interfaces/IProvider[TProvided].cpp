namespace Platform::Interfaces
{
    template <typename TProvided> class IProvider<TProvided>
    {
    public:
        virtual TProvided Get() = 0;
    };
}
