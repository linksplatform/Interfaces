namespace Platform.Interfaces
{
    /// <summary>
    /// Defines the provider of objects for which an argument must be specified.
    /// Определяет поставщика объектов, для получения которых необходимо указать аргумент.
    /// </summary>
    /// <typeparam name="TProvided">Type of provided objects. Тип предоставляемых объектов.</typeparam>
    /// <typeparam name="TArgument">Argument type. Тип аргумента.</typeparam>
    public interface IProvider<out TProvided, in TArgument>
    {
        /// <summary>
        /// Provides an object.
        /// </summary>
        /// <param name="argument">The argument required to acquire the object. Аргумент, необходимый для получения объекта.</param>
        /// <returns>The object.</returns>
        TProvided Get(TArgument argument);
    }
}
