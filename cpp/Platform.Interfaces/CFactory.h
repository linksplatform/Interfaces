#pragma once

#include <concepts>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires a factory that creates the specified product type.</para>
  /// <para>Требует фабрику, создающую объект заданного типа.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TProduct">
  /// <para>The type of the created product.</para>
  /// <para>Тип создаваемого объекта.</para>
  /// </typeparam>
  template <typename TSelf, typename TProduct>
  concept CFactory = requires(TSelf self) {
    { self.Create() } -> std::same_as<TProduct>;
  };
}  // namespace Platform::Interfaces
