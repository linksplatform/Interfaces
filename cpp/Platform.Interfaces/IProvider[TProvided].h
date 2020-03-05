namespace Platform::Interfaces
{
    template <typename...> class IProvider; template <typename TProvided> class IProvider<TProvided>
    {
    public:
        virtual TProvided Get() = 0;
    };
}
