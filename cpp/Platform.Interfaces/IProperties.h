#pragma once

namespace Platform::Interfaces {
  template <typename...>
  struct IProperties;

  /// <summary>
  /// <para>Defines a properties operator that is able to get or set values of properties of a object of a specific type.</para>
  /// <para>Определяет оператор свойств, который может получать или устанавливать значения свойств объекта определенного типа.</para>
  /// </summary>
  /// <typeparam name="TObject">
  /// <para>Object type.</para>
  /// <para>Тип объекта.</para>
  /// </typeparam>
  /// <typeparam name="TProperty">
  /// <para>Property reference type.</para>
  /// <para>Тип ссылки на свойство.</para>
  /// </typeparam>
  /// <typeparam name="TValue">
  /// <para>Property value type.</para>
  /// <para>Тип значения свойства.</para>
  /// </typeparam>
  template <typename TObject, typename TProperty, typename TValue>
  struct IProperties<TObject, TProperty, TValue> {
    /// <summary>
    /// <para>Gets the value of the property in the specified object.</para>
    /// <para>Получает значение свойства в указанном объекте.</para>
    /// </summary>
    /// <param name="object">
    /// <para>The object reference.</para>
    /// <para>Ссылка на объект.</para>
    /// </param>
    /// <param name="property">
    /// <para>The property reference.</para>
    /// <para>Ссылка на свойство.</para>
    /// </param>
    /// <returns>
    /// <para>The value of the property.</para>
    /// <para>Значение свойства.</para>
    /// </returns>
    virtual TValue GetValue(TObject object, TProperty property) = 0;

    /// <summary>
    /// <para>Sets the value of a property in the specified object.</para>
    /// <para>Устанавливает значение свойства в указанном объекте.</para>
    /// </summary>
    /// <param name="object">
    /// <para>The object reference.</para>
    /// <para>Ссылка на объект.</para>
    /// </param>
    /// <param name="property">
    /// <para>The property reference.</para>
    /// <para>Ссылка на свойство.</para>
    /// </param>
    /// <param name="value">
    /// <para>The value.</para>
    /// <para>Значение.</para>
    /// </param>
    virtual void SetValue(TObject object, TProperty property, TValue value) = 0;

    virtual ~IProperties() = default;
  };
}  // namespace Platform::Interfaces
