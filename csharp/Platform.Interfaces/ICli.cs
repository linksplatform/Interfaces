namespace Platform.Interfaces;

/// <summary>
/// <para>Defines command line interfaces.</para>
/// <para>Определяет интерфейс коммандной строки.</para>
/// </summary>
/// <typeparam name="TLinkAddress"></typeparam>
public interface ICli<TLinkAddress>
{
    /// <summary>
    /// <para>Runs command line interface.</para>
    /// <para>Запускает интерфейс коммандной строки.</para>
    /// </summary>
    /// <param name="args">
    /// <para>Command line arguments.</para>
    /// <para>Аргументы коммандной строки/</para>
    /// </param>
    void Run(params string[] args);
}
