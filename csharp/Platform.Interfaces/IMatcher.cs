namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines a matcher that contains a specific method for determining whether the argument matches or not.</para>
    /// <para>Определяет объект который проверяет соответствие и содержит конкретный метод для определения, соответствует ли аргумент или нет.</para>
    /// </summary>
    /// <typeparam name="TArgument">
    /// <para>Argument type.</para>
    /// <para>Тип аргумента.</para>
    /// </typeparam>
    public interface IMatcher<in TArgument>
    {
        /// <summary>
        /// <para>Determines whether the argument matches.</para>
        /// <para>Определяет, соответствует ли аргумент.</para>
        /// </summary>
        /// <param name="argument">
        /// <para>The argument.</para>
        /// <para>Аргумент.</para>
        /// </param>
        /// <returns>
        /// <para>A value that determines whether the argument matches.</para>
        /// <para>Значение, определяющие соответствует ли аргумент.</para>
        /// </returns>
        bool IsMatched(TArgument argument);
    }
}
