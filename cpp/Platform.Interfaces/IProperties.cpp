namespace Platform::Interfaces
{
    template <typename TObject, typename TProperty, typename TValue> class IProperties
    {
    public:
        virtual TValue GetValue(TObject @object, TProperty property) = 0;

        virtual void SetValue(TObject @object, TProperty property, TValue value) = 0;
    };
}
