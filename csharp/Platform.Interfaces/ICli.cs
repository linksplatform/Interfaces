namespace Platform.Interfaces;

/// <summary>
/// <para>Defines command line interfaces for command that interacts with an operating system.</para>
/// <para>Определяет интерфейс командной строки, для команды взаимодействующей с операционной системой.</para>
/// </summary>
public interface ICli
{
    /// <summary>
    /// <para>Runs a command.</para>
    /// <para>Запускает команду.</para>
    /// </summary>
    /// <param name="args">
    /// <para>Arguments for a command.</para>
    /// <para>Аргументы для команды.</para>
    /// </param>
    /// <returns>
    /// <para>Returns command's exit code.</para>
    /// <para>Возвращает код выхода команды.</para>
    /// </returns>
    int Run(params string[] args);
}
