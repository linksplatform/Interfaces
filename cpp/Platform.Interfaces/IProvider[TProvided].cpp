namespace Platform::Interfaces
{
    template <typename TProvided> class IProvider
    {
    public:
        virtual TProvided Get() = 0;
    }
}
