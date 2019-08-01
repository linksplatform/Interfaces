namespace Platform.Interfaces
{
    /// <summary>
    /// Defines the provider of objects.
    /// Определяет поставщика объектов.
    /// </summary>
    /// <typeparam name="TProvided">Type of provided object. Тип предоставляемого объекта.</typeparam>
    public interface IProvider<TProvided>
    {
        /// <summary>
        /// Provides an object.
        /// </summary>
        /// <returns>The provided object.</returns>
        TProvided Get();
    }
}
