#pragma once

#include <concepts>
#include <string>
#include <vector>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires a command runner that returns an exit code.</para>
  /// <para>Требует исполнитель команды, возвращающий код завершения.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <typename TSelf>
  concept CCli = requires(TSelf self, const std::vector<std::string>& args) {
    { self.Run(args) } -> std::same_as<int>;
  };
}  // namespace Platform::Interfaces
