namespace Platform::Interfaces
{
    interface IProperty<in TObject, TValue> : ISetter<TValue, TObject>, IProvider<TValue, TObject>
    {
    }
}
