namespace Platform::Interfaces
{
    template <typename TObject, typename TValue> class IProperty : public ISetter<TValue, TObject>, public IProvider<TValue, TObject>
    {
    public:
    };
}
