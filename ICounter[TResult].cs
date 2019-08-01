namespace Platform.Interfaces
{
    /// <summary>
    /// Defines a counter.
    /// Определяет счетчик.
    /// </summary>
    /// <typeparam name="TResult">The count result type. Тип результата подсчета.</typeparam>
    public interface ICounter<TResult>
    {
        /// <summary>
        /// Performs a count.
        /// Выполняет посчёт.
        /// </summary>
        /// <returns>The count result. Результат подсчёта.</returns>
        TResult Count();
    }
}
