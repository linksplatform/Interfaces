namespace Platform.Interfaces
{
    /// <summary>
    /// Defines a properties operator that is able to get or set values of properties of a object of a specific type.
    /// Определяет оператор свойств, который может получать или установливать значения свойств объекта определенного типа.
    /// </summary>
    /// <typeparam name="TObject">Object type. Тип объекта.</typeparam>
    /// <typeparam name="TProperty">Property reference type. Тип ссылки на свойство.</typeparam>
    /// <typeparam name="TValue">Property value type. Тип значения свойства.</typeparam>
    public interface IPropertiesOperator<in TObject, in TProperty, TValue>
    {
        /// <summary>
        /// Gets the value of the property in the specified object.
        /// Получает значение свойства в указанном объекте.
        /// </summary>
        /// <param name="object">The object reference. Ссылка на объект.</param>
        /// <param name="property">The property reference. Ссылка на свойство.</param>
        /// <returns>The value of the property. Значение свойства.</returns>
        TValue GetValue(TObject @object, TProperty property);

        /// <summary>
        /// Sets the value of a property in the specified object.
        /// Устанавливает значение свойства в указанном объекте.
        /// </summary>
        /// <param name="object">The object reference. Ссылка на объект.</param>
        /// <param name="property">The property reference. Ссылка на свойство.</param>
        /// <param name="value">The value. Значение.</param>
        void SetValue(TObject @object, TProperty property, TValue value);
    }
}
