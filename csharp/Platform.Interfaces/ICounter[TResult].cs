namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines a counter.</para>
    /// <para>Определяет счётчик.</para>
    /// </summary>
    /// <typeparam name="TResult">
    /// <para>The count result type.</para>
    /// <para>Тип результата подсчёта.</para>
    /// </typeparam>
    public interface ICounter<out TResult>
    {
        /// <summary>
        /// <para>Performs a count.</para>
        /// <para>Выполняет подсчёт.</para>
        /// </summary>
        /// <returns>
        /// <para>The count result.</para>
        /// <para>Результат подсчёта.</para>
        /// </returns>
        TResult Count();
    }
}
