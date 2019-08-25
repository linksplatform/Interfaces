namespace Platform.Interfaces
{
    /// <summary>
    /// <para>Defines a converter between two values of the same type.</para>
    /// <para>Определяет конвертер между двумя значениями одного типа.</para>
    /// </summary>
    /// <typeparam name="T"><para>Type of value to convert.</para><para>Тип преобразуемого значения.</para></typeparam>
    public interface IConverter<T> : IConverter<T, T>
    {
    }
}
