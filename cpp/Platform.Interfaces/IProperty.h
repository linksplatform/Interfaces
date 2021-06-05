namespace Platform::Interfaces
{
    template<typename Self, typename TObject, typename TValue>
    concept IProperty = ISetter<Self, TValue, TObject> && IProvider<Self, TValue, TObject>;
}
