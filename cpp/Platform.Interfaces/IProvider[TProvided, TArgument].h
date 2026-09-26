#pragma once

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Declares the primary provider interface template for its supported specializations.</para>
  /// <para>Объявляет основной шаблон интерфейса поставщика для поддерживаемых специализаций.</para>
  /// </summary>
  template <typename...>
  struct IProvider;

  /// <summary>
  /// <para>Defines the provider of objects/values for which an argument must be specified.</para>
  /// <para>Определяет поставщика объектов/значений, для получения которых необходимо указать аргумент.</para>
  /// </summary>
  /// <typeparam name="TProvided">
  /// <para>Type of provided objects/values.</para>
  /// <para>Тип предоставляемых объектов/значений.</para>
  /// </typeparam>
  /// <typeparam name="TArgument">
  /// <para>Argument type.</para>
  /// <para>Тип аргумента.</para>
  /// </typeparam>
  template <typename TProvided, typename TArgument>
  struct IProvider<TProvided, TArgument> {
    /// <summary>
    /// <para>Provides an object(s)/value(s).</para>
    /// <para>Предоставляет объект(ы)/значение(я).</para>
    /// </summary>
    /// <param name="argument">
    /// <para>The argument required to acquire the object(s)/value(s).</para>
    /// <para>Аргумент, необходимый для получения объекта(ов)/значения(ий).</para>
    /// </param>
    /// <returns>
    /// <para>The object(s)/value(s).</para>
    /// <para>Объект(ы)/значение(я).</para>
    /// </returns>
    virtual TProvided Get(TArgument argument) = 0;

    /// <summary>
    /// <para>Destroys the interface instance.</para>
    /// <para>Уничтожает экземпляр интерфейса.</para>
    /// </summary>
    virtual ~IProvider() = default;
  };
}  // namespace Platform::Interfaces
