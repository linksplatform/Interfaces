namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines a specific property operator that is able to get or set values of that property.</para>
    /// <para>Определяет оператор определённого свойства, который может получать или установливать его значения.</para>
    /// </summary>
    /// <typeparam name="TObject"><para>Object type.</para><para>Тип объекта.</para></typeparam>
    /// <typeparam name="TValue"><para>Property value type.</para><para>Тип значения свойства.</para></typeparam>
    public interface IPropertyOperator<in TObject, TValue> : IProvider<TValue, TObject>
    {
        /// <summary>
        /// <para>Sets the value of a specific property in the specified object.</para>
        /// <para>Устанавливает значение определённого свойства в указанном объекте.</para>
        /// </summary>
        /// <param name="object"><para>The object reference.</para><para>Ссылка на объект.</para></param>
        /// <param name="value"><para>The value.</para><para>Значение.</para></param>
        void Set(TObject @object, TValue value);
    }
}
