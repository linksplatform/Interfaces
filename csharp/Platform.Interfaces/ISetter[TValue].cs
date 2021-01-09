namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines an setter that sets the passed value as a new state.</para>
    /// <para>Определяет установщик, который устанавливает переданное значение в качестве нового состояния.</para>
    /// </summary>
    /// <typeparam name="TValue"><para>Type of set value.</para><para>Тип устанавливаемого значения.</para></typeparam>
    public interface ISetter<in TValue>
    {
        /// <summary>
        /// <para>Sets the value of a specific property in the specified object.</para>
        /// <para>Устанавливает значение определённого свойства в указанном объекте.</para>
        /// </summary>
        /// <param name="value"><para>The value.</para><para>Значение.</para></param>
        void Set(TValue value);
    }
}
