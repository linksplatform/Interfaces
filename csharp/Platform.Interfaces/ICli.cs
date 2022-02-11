namespace Platform.Interfaces;

/// <summary>
/// <para>Defines command line interfaces for command that interacts with an operational system.</para>
/// <para>Определяет интерфейс командной строки, для команды взаимодействующей с операционной системой.</para>
/// </summary>
public interface ICli
{
    /// <summary>
    /// <para>Runs command.</para>
    /// <para>Запускает команду.</para>
    /// </summary>
    /// <param name="args">
    /// <para>Arguments for command.</para>
    /// <para>Аргументы для команды.</para>
    /// </param>
    /// <returns>
    /// <para>Returns an command exit code.</para>
    /// <para>Возвращает код выхода команды.</para>
    /// </returns>
    int Run(params string[] args);
}
