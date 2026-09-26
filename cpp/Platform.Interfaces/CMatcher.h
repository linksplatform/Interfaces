#pragma once

#include <concepts>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires a matcher that reports whether a candidate satisfies its rule.</para>
  /// <para>Требует объект, сообщающий, соответствует ли кандидат правилу сопоставления.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TCandidate">
  /// <para>The type of the candidate to match.</para>
  /// <para>Тип проверяемого кандидата.</para>
  /// </typeparam>
  template <typename TSelf, typename TCandidate>
  concept CMatcher = requires(TSelf self, TCandidate candidate) {
    { self.IsMatched(candidate) } -> std::same_as<bool>;
  };
}  // namespace Platform::Interfaces
