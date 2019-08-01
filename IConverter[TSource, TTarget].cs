namespace Platform.Interfaces
{
    /// <summary>
    /// Defines a converter between two types (TSource and TTarget).
    /// Определяет конвертер между двумя типами (исходным TSource и целевым TTarget).
    /// </summary>
    /// <typeparam name="TSource">Source type of conversion. Исходный тип конверсии.</typeparam>
    /// <typeparam name="TTarget">Target type of conversion. Целевой тип конверсии.</typeparam>
    public interface IConverter<in TSource, out TTarget>
    {
        /// <summary>
        /// Converts the value of the source type (TSource) to the value of the target type.
        /// Конвертирует значение исходного типа (TSource) в значение целевого типа.
        /// </summary>
        /// <param name="source">The source type value (TSource). Значение исходного типа (TSource).</param>
        /// <returns>The value is converted to the target type (TTarget). Значение ковертированное в целевой тип (TTarget).</returns>
        TTarget Convert(TSource source);
    }
}
