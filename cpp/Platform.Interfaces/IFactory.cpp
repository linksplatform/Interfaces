namespace Platform::Interfaces
{
    template <typename TProduct> class IFactory
    {
    public:
        virtual TProduct Create() = 0;
    };
}
