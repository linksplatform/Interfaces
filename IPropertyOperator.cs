namespace Platform.Interfaces
{
    /// <summary>
    /// Defines a specific property operator that is able to get or set values of that property.
    /// Определяет оператор определённого свойства, который может получать или установливать его значения.
    /// </summary>
    /// <typeparam name="TObject">Object type. Тип объекта.</typeparam>
    /// <typeparam name="TValue">Property value type. Тип значения свойства.</typeparam>
    public interface IPropertyOperator<in TObject, TValue> : IProvider<TValue, TObject>
    {
        /// <summary>
        /// Sets the value of a specific property in the specified object.
        /// Устанавливает значение определённого свойства в указанном объекте.
        /// </summary>
        /// <param name="object">The object reference. Ссылка на объект.</param>
        /// <param name="value">The value. Значение.</param>
        void Set(TObject @object, TValue value);
    }
}
