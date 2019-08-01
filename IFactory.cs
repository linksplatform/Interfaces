namespace Platform.Interfaces
{
    /// <summary>
    /// Defines a factory that produces instances of a specific type.
    /// Определяет фабрику, которая производит экземпляры определенного типа.
    /// </summary>
    /// <typeparam name="TProduct">Type of produced instances. Тип производимых экземпляров.</typeparam>
    public interface IFactory<out TProduct>
    {
        /// <summary>
        /// Creates an instance of TProduct type.
        /// Создает экземпляр типа TProduct.
        /// </summary>
        /// <returns>The instance of TProduct type. Экземпляр типа TProduct.</returns>
        TProduct Create();
    }
}
