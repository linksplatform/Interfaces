namespace Platform.Interfaces
{
    /// <summary>
    /// Defines a counter that requires passing an argument to perform a count.
    /// Определяет счетчик, который требует передачи аргумента для выполнения подсчёта.
    /// </summary>
    /// <typeparam name="TArgument">The argument type. Тип аргумента.</typeparam>
    /// <typeparam name="TResult">The count result type. Тип результата подсчета.</typeparam>
    public interface ICounter<TArgument, TResult>
    {
        /// <summary>
        /// Performs a count that requires passing an argument.
        /// Выполняет посчёт, для которого требуется передача аргумент.
        /// </summary>
        /// <param name="argument">The argument. Аргумент.</param>
        /// <returns>The count result. Результат подсчёта.</returns>
        TResult Count(TArgument argument);
    }
}
