namespace Platform.Interfaces
{
    public interface IFactory<TProduct>
    {
        TProduct Create();
    }
}
