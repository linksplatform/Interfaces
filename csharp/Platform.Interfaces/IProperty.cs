namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines a specific property operator-object that is able to get or set values of that property.</para>
    /// <para>Определяет объект-оператор определённого свойства, который может получать или установливать его значения.</para>
    /// </summary>
    /// <typeparam name="TObject"><para>Object type.</para><para>Тип объекта.</para></typeparam>
    /// <typeparam name="TValue"><para>Property value type.</para><para>Тип значения свойства.</para></typeparam>
    public interface IProperty<in TObject, TValue> : ISetter<TValue, TObject>, IProvider<TValue, TObject>
    {
    }
}
