namespace Platform::Interfaces
{
    interface IProperties<in TObject, in TProperty, TValue>
    {
        TValue GetValue(TObject @object, TProperty property);

        void SetValue(TObject @object, TProperty property, TValue value);
    }
}
