namespace Platform.Interfaces
{
    /// <summary>
    /// Defines a criterion matcher, that contains a specific method of determining whether the argument matches criterion or not.
    /// Определяет объект который проверяет соответствие критерию и содержащий конкретный метод определения, соответствует ли аргумент критерию или нет.
    /// </summary>
    /// <typeparam name="TArgument">Argument type. Тип аргумента.</typeparam>
    public interface ICriterionMatcher<TArgument>
    {
        /// <summary>
        /// Determines whether the argument matches the criterion.
        /// Определяет, соответствует ли аргумент критерию.
        /// </summary>
        /// <param name="argument">The argument. Аргумент.</param>
        /// <returns>A value that determines whether the argument matches the criterion. Значение, определяющие соответствует ли аргумент критерию.</returns>
        bool IsMatched(TArgument argument);
    }
}
