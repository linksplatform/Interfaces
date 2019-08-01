namespace Platform.Interfaces
{
    /// <summary>
    /// Defines an incrementer that increments any number with a specific type.
    /// Определяет инкрементер, который выполняет приращение любого числа определенного типа.
    /// </summary>
    /// <typeparam name="TNumber">Type of incremented number. Тип увеличиваемого числа.</typeparam>
    public interface IIncrementer<TNumber>
    {
        /// <summary>
        /// Increments the number by a specific value.
        /// Увеличивает число на определённое значение.
        /// </summary>
        /// <param name="number">The number to be incremented.</param>
        /// <returns>The incremented number. Увеличенное число.</returns>
        TNumber Increment(TNumber number);
    }
}
