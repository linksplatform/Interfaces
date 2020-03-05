namespace Platform::Interfaces
{
    template <typename...> class IFactory; template <typename TProduct> class IFactory<TProduct>
    {
    public:
        virtual TProduct Create() = 0;
    };
}
