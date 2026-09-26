#pragma once

#include <concepts>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires an operator that gets and sets a named property of an object.</para>
  /// <para>Требует оператор, получающий и устанавливающий указанное свойство объекта.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TObject">
  /// <para>The object type.</para>
  /// <para>Тип объекта.</para>
  /// </typeparam>
  /// <typeparam name="TProperty">
  /// <para>The property reference type.</para>
  /// <para>Тип ссылки на свойство.</para>
  /// </typeparam>
  /// <typeparam name="TValue">
  /// <para>The value type.</para>
  /// <para>Тип значения.</para>
  /// </typeparam>
  template <typename TSelf, typename TObject, typename TProperty, typename TValue>
  concept CProperties = requires(TSelf self, TObject& object, TProperty property, TValue value) {
    { self.GetValue(object, property) } -> std::same_as<TValue>;

    { self.SetValue(object, property, value) } -> std::same_as<void>;
  };
}  // namespace Platform::Interfaces
