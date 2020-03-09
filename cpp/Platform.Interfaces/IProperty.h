namespace Platform::Interfaces
{
    template <typename ...> class IProperty;
    template <typename TObject, typename TValue> class IProperty<TObject, TValue> : public ISetter<TValue, TObject>, IProvider<TValue, TObject>
    {
    public:
    };
}
