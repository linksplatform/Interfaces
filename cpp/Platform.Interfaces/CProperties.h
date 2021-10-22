namespace Platform::Interfaces
{
    template <typename Self, typename TObject, typename TProperty, typename TValue>
    concept CProperties = requires(Self self, TObject object, TProperty property, TValue value)
    {
        { self.GetValue(object, property) } -> std::same_as<TValue>;

        { self.SetValue(object, property, value) } -> std::same_as<void>;
    };
}
