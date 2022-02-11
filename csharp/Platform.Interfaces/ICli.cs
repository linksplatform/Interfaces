namespace Platform.Interfaces;

/// <summary>
/// <para>Defines command line interfaces.</para>
/// <para>Определяет интерфейс коммандной строки.</para>
/// </summary>
public interface ICli
{
    /// <summary>
    /// <para>Runs commands.</para>
    /// <para>Запускает команду.</para>
    /// </summary>
    /// <param name="args">
    /// <para>Command line arguments.</para>
    /// <para>Аргументы для команды.</para>
    /// </param>
    /// <returns>
    /// <para>Returns an exit code.</para>
    /// <para>Возвращает код выхода.</para>
    /// </returns>
    int Run(params string[] args);
}
