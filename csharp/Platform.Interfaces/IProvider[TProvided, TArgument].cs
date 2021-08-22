namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines the provider of objects/values for which an argument must be specified.</para>
    /// <para>Определяет поставщика объектов/значений, для получения которых необходимо указать аргумент.</para>
    /// </summary>
    /// <typeparam name="TProvided">
    /// <para>Type of provided objects/values.</para>
    /// <para>Тип предоставляемых объектов/значений.</para>
    /// </typeparam>
    /// <typeparam name="TArgument">
    /// <para>Argument type.</para>
    /// <para>Тип аргумента.</para>
    /// </typeparam>
    public interface IProvider<out TProvided, in TArgument>
    {
        /// <summary>
        /// <para>Provides an object(s)/value(s).</para>
        /// <para>Предоставляет объект(ы)/значение(я).</para>
        /// </summary>
        /// <param name="argument">
        /// <para>The argument required to acquire the object(s)/value(s).</para>
        /// <para>Аргумент, необходимый для получения объекта(ов)/значения(ий).</para>
        /// </param>
        /// <returns>
        /// <para>The object(s)/value(s).</para>
        /// <para>Объект(ы)/значение(я).</para>
        /// </returns>
        TProvided Get(TArgument argument);
    }
}
