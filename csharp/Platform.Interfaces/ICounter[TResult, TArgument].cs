namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines a counter that requires passing an argument to perform a count.</para>
    /// <para>Определяет счётчик, который требует передачи аргумента для выполнения подсчёта.</para>
    /// </summary>
    /// <typeparam name="TArgument"><para>The argument type.</para><para>Тип аргумента.</para></typeparam>
    /// <typeparam name="TResult"><para>The count result type.</para><para>Тип результата подсчёта.</para></typeparam>
    public interface ICounter<out TResult, in TArgument>
    {
        /// <summary>
        /// <para>Performs a count that requires passing an argument.</para>
        /// <para>Выполняет подсчёт, для которого требуется передача аргумента.</para>
        /// </summary>
        /// <param name="argument"><para>The argument.</para><para>Аргумент.</para></param>
        /// <returns><para>The count result.</para><para>Результат подсчёта.</para></returns>
        TResult Count(TArgument argument);
    }
}
