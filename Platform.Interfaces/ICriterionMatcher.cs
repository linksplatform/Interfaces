namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines a criterion matcher, that contains a specific method of determining whether the argument matches criterion or not.</para>
    /// <para>Определяет объект который проверяет соответствие критерию и содержащий конкретный метод определения, соответствует ли аргумент критерию или нет.</para>
    /// </summary>
    /// <typeparam name="TArgument"><para>Argument type.</para><para>Тип аргумента.</para></typeparam>
    public interface ICriterionMatcher<in TArgument>
    {
        /// <summary>
        /// <para>Determines whether the argument matches the criterion.</para>
        /// <para>Определяет, соответствует ли аргумент критерию.</para>
        /// </summary>
        /// <param name="argument"><para>The argument.</para><para>Аргумент.</para></param>
        /// <returns><para>A value that determines whether the argument matches the criterion.</para><para>Значение, определяющие соответствует ли аргумент критерию.</para></returns>
        bool IsMatched(TArgument argument);
    }
}
