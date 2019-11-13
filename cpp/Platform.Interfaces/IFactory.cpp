namespace Platform::Interfaces
{
    interface IFactory<out TProduct>
    {
        TProduct Create();
    }
}
