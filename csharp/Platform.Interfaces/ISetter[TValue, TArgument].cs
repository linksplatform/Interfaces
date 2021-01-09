namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines an setter that requires an argument to set the passed value as a new state.</para>
    /// <para>Определяет установщик, которому для установки переданного значения в качестве нового состояния требуется аргумент.</para>
    /// </summary>
    /// <typeparam name="TValue"><para>Type of set value.</para><para>Тип устанавливаемого значения.</para></typeparam>
    /// <typeparam name="TArgument"><para>The argument type.</para><para>Тип аргумента.</para></typeparam>
    public interface ISetter<in TValue, in TArgument>
    {
        /// <summary>
        /// <para>Sets the value of a specific property in the specified object.</para>
        /// <para>Устанавливает значение определённого свойства в указанном объекте.</para>
        /// </summary>
        /// <param name="argument"><para>The argument.</para><para>Аргумент.</para></param>
        /// <param name="value"><para>The value.</para><para>Значение.</para></param>
        void Set(TArgument argument, TValue value);
    }
}
