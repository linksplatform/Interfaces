namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines an incrementer that increments any number with a specific type.</para>
    /// <para>Определяет инкрементер, который выполняет приращение любого числа определенного типа.</para>
    /// </summary>
    /// <typeparam name="TNumber"><para>Type of incremented number.</para><para>Тип увеличиваемого числа.</para></typeparam>
    public interface IIncrementer<TNumber>
    {
        /// <summary>
        /// <para>Increments the number by a specific value.</para>
        /// <para>Увеличивает число на определённое значение.</para>
        /// </summary>
        /// <param name="number"><para>The number to be incremented.</para><para>Увеличиваемое число.</para></param>
        /// <returns><para>The incremented number.</para><para>Увеличенное число.</para></returns>
        TNumber Increment(TNumber number);
    }
}
