namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines the provider of objects/values.</para>
    /// <para>Определяет поставщика объектов/значений.</para>
    /// </summary>
    /// <typeparam name="TProvided">
    /// <para>Type of provided object/value.</para>
    /// <para>Тип предоставляемого объекта/значения.</para>
    /// </typeparam>
    public interface IProvider<out TProvided>
    {
        /// <summary>
        /// <para>Provides an object(s)/value(s).</para>
        /// <para>Предоставляет объект(ы)/значение(я).</para>
        /// </summary>
        /// <returns>
        /// <para>The object(s)/value(s).</para>
        /// <para>Объект(ы)/значение(я).</para>
        /// </returns>
        TProvided Get();
    }
}
