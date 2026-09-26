namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines a matcher that determines whether a candidate satisfies its matching rule.</para>
    /// <para>Определяет объект, который проверяет, соответствует ли кандидат правилу сопоставления.</para>
    /// </summary>
    /// <typeparam name="TCandidate">
    /// <para>Type of the value being tested for a match.</para>
    /// <para>Тип значения, проверяемого на соответствие.</para>
    /// </typeparam>
    public interface IMatcher<in TCandidate>
    {
        /// <summary>
        /// <para>Determines whether the candidate satisfies the matching rule.</para>
        /// <para>Определяет, соответствует ли кандидат правилу сопоставления.</para>
        /// </summary>
        /// <param name="candidate">
        /// <para>The value to test.</para>
        /// <para>Проверяемое значение.</para>
        /// </param>
        /// <returns>
        /// <para>Whether the candidate satisfies the matching rule.</para>
        /// <para>Соответствует ли кандидат правилу сопоставления.</para>
        /// </returns>
        bool IsMatched(TCandidate candidate);
    }
}
