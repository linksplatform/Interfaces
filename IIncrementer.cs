namespace Platform.Interfaces
{
    public interface IIncrementer<T>
    {
        T Increment(T number);
    }
}
