namespace Platform.Interfaces
{
    public interface IConverter<TSource, TTarget>
    {
        TTarget Convert(TSource source);
    }
}
