#pragma once

namespace Platform::Interfaces {
  template <typename...>
  struct IProvider;

  /// <summary>
  /// <para>Defines the provider of objects/values.</para>
  /// <para>Определяет поставщика объектов/значений.</para>
  /// </summary>
  /// <typeparam name="TProvided">
  /// <para>Type of provided object/value.</para>
  /// <para>Тип предоставляемого объекта/значения.</para>
  /// </typeparam>
  template <typename TProvided>
  struct IProvider<TProvided> {
    /// <summary>
    /// <para>Provides an object(s)/value(s).</para>
    /// <para>Предоставляет объект(ы)/значение(я).</para>
    /// </summary>
    /// <returns>
    /// <para>The object(s)/value(s).</para>
    /// <para>Объект(ы)/значение(я).</para>
    /// </returns>
    virtual TProvided Get() = 0;

    virtual ~IProvider() = default;
  };
}  // namespace Platform::Interfaces
