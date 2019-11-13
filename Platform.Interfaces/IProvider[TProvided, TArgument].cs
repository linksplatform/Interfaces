namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines the provider of objects for which an argument must be specified.</para>
    /// <para>Определяет поставщика объектов, для получения которых необходимо указать аргумент.</para>
    /// </summary>
    /// <typeparam name="TProvided"><para>Type of provided objects.</para><para>Тип предоставляемых объектов.</para></typeparam>
    /// <typeparam name="TArgument"><para>Argument type.</para><para>Тип аргумента.</para></typeparam>
    public interface IProvider<out TProvided, in TArgument>
    {
        /// <summary>
        /// <para>Provides an object.</para>
        /// <para>Предоставляет объект.</para>
        /// </summary>
        /// <param name="argument"><para>The argument required to acquire the object.</para><para>Аргумент, необходимый для получения объекта.</para></param>
        /// <returns><para>The object.</para><para>Объект.</para></returns>
        TProvided Get(TArgument argument);
    }
}
