namespace Platform::Interfaces
{
    template <typename Self, typename TProduct>
    concept IFactory = requires(Self self)
    {
        {self.Create()} -> std::same_as<TProduct>;
    };
}
