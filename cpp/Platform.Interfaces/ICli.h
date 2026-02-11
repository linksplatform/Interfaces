#pragma once

#include <string>
#include <vector>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Defines command line interfaces for command that interacts with an operating system.</para>
  /// <para>Определяет интерфейс командной строки, для команды взаимодействующей с операционной системой.</para>
  /// </summary>
  struct ICli {
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
    virtual int Run(const std::vector<std::string>& args) = 0;

    virtual ~ICli() = default;
  };
}  // namespace Platform::Interfaces