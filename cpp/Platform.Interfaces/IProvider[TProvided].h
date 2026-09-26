#pragma once

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Declares the primary provider interface template for its supported specializations.</para>
  /// <para>Объявляет основной шаблон интерфейса поставщика для поддерживаемых специализаций.</para>
  /// </summary>
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

    /// <summary>
    /// <para>Destroys the interface instance.</para>
    /// <para>Уничтожает экземпляр интерфейса.</para>
    /// </summary>
    virtual ~IProvider() = default;
  };
}  // namespace Platform::Interfaces
