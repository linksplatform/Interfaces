namespace Platform.Interfaces;

/// <summary>
/// <para>Defines command line interfaces.</para>
/// <para>Определяет интерфейс коммандной строки.</para>
/// </summary>
public interface ICli
{
    /// <summary>
    /// <para>Runs commands.</para>
    /// <para>Запускает коммандну.</para>
    /// </summary>
    /// <param name="args">
    /// <para>Command line arguments.</para>
    /// <para>Аргументы коммандной строки/</para>
    /// </param>
    void Run(params string[] args);
}
