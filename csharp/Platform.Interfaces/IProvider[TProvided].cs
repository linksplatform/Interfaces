namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines the provider of objects/values.</para>
    /// <para>Определяет поставщика объектов/значений.</para>
    /// </summary>
    /// <typeparam name="TProvided"><para>Type of provided object/value.</para><para>Тип предоставляемого объекта/значения.</para></typeparam>
    public interface IProvider<out TProvided>
    {
        /// <summary>
        /// <para>Provides an object/value.</para>
        /// <para>Предоставляет объект/значение.</para>
        /// </summary>
        /// <returns><para>Object/value.</para><para>Объект/значение.</para></returns>
        TProvided Get();
    }
}
