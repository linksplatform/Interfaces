namespace Platform.Interfaces
{ 
    /// <summary>
    /// <para>Defines a factory that produces instances of a specific type.</para>
    /// <para>Определяет фабрику, которая производит экземпляры определенного типа.</para>
    /// </summary>
    /// <typeparam name="TProduct">
    /// <para>Type of produced instances.</para>
    /// <para>Тип производимых экземпляров.</para>
    /// </typeparam>
    public interface IFactory<out TProduct>
    {
        /// <summary>
        /// <para>Creates an instance of <typeparamref name="TProduct"/> type.</para>
        /// <para>Создает экземпляр типа <typeparamref name="TProduct"/>.</para>
        /// </summary>
        /// <returns>
        /// <para>The instance of <typeparamref name="TProduct"/> type.</para>
        /// <para>Экземпляр типа <typeparamref name="TProduct"/>.</para>
        /// </returns>
        TProduct Create();
    }
}
