namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines the provider of objects.</para>
    /// <para>Определяет поставщика объектов.</para>
    /// </summary>
    /// <typeparam name="TProvided"><para>Type of provided object.</para><para>Тип предоставляемого объекта.</para></typeparam>
    public interface IProvider<out TProvided>
    {
        /// <summary>
        /// <para>Provides an object.</para>
        /// </summary>
        /// <returns><para>The provided object.</para></returns>
        TProvided Get();
    }
}
