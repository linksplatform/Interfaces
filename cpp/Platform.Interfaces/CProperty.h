namespace Platform::Interfaces
{
    template<typename Self, typename TObject, typename TValue>
    concept CProperty = CSetter<Self, TValue, TObject> && CProvider<Self, TValue, TObject>;
}
