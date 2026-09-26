#pragma once

#include <concepts>
#include <type_traits>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires a link with endpoints, a value type and an empty-state query.</para>
  /// <para>Требует связь с концами, типом значения и проверкой пустого состояния.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <typename TSelf>
  concept CLink = requires(TSelf self) {
    { self.empty() } -> std::same_as<bool>;
    typename TSelf::value_type;
    { self.begin } -> std::same_as<typename TSelf::value_type&>;
    { self.end } -> std::same_as<typename TSelf::value_type&>;
  };

  /// <summary>
  /// <para>Exposes the value type of a link.</para>
  /// <para>Предоставляет тип значения связи.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  template <CLink TSelf>
  struct Link {
    /// <summary>
    /// <para>The value type of the link.</para>
    /// <para>Тип значения связи.</para>
    /// </summary>
    using value_type = typename TSelf::value_type;
  };
}  // namespace Platform::Interfaces
