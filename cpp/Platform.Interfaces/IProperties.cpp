namespace Platform::Interfaces
{
    template <typename...> class IProperties; template <typename TObject, typename TProperty, typename TValue> class IProperties<TObject, TProperty, TValue>
    {
    public:
        virtual TValue GetValue(TObject object, TProperty property) = 0;

        virtual void SetValue(TObject object, TProperty property, TValue value) = 0;
    };
}
