namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines the provider of objects/values for which an argument must be specified.</para>
    /// <para>Определяет поставщика объектов/значений, для получения которых необходимо указать аргумент.</para>
    /// </summary>
    /// <typeparam name="TProvided"><para>Type of provided objects/values.</para><para>Тип предоставляемых объектов/значений.</para></typeparam>
    /// <typeparam name="TArgument"><para>Argument type.</para><para>Тип аргумента.</para></typeparam>
    public interface IProvider<out TProvided, in TArgument>
    {
        /// <summary>
        /// <para>Provides an object/value.</para>
        /// <para>Предоставляет объект/значение.</para>
        /// </summary>
        /// <param name="argument"><para>The argument required to acquire the object/value.</para><para>Аргумент, необходимый для получения объекта/значения.</para></param>
        /// <returns><para>The object/value.</para><para>Объект/значение.</para></returns>
        TProvided Get(TArgument argument);
    }
}
