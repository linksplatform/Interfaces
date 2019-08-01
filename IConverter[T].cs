namespace Platform.Interfaces
{
    /// <summary>
    /// Defines a converter between two values of the same type.
    /// Определяет конвертер между двумя значениями одного типа.
    /// </summary>
    /// <typeparam name="T">Type of value to convert. Тип преобразуемого значения.</typeparam>
    public interface IConverter<T> : IConverter<T, T>
    {
    }
}
