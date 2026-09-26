#pragma once

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Declares the primary setter interface template for its supported specializations.</para>
  /// <para>Объявляет основной шаблон интерфейса установщика для поддерживаемых специализаций.</para>
  /// </summary>
  template <typename...>
  struct ISetter;

  /// <summary>
  /// <para>Defines an setter that requires an argument to set the passed value as a new state.</para>
  /// <para>Определяет установщик, которому для установки переданного значения в качестве нового состояния требуется аргумент.</para>
  /// </summary>
  /// <typeparam name="TValue">
  /// <para>Type of set value.</para>
  /// <para>Тип устанавливаемого значения.</para>
  /// </typeparam>
  /// <typeparam name="TArgument">
  /// <para>The argument type.</para>
  /// <para>Тип аргумента.</para>
  /// </typeparam>
  template <typename TValue, typename TArgument>
  struct ISetter<TValue, TArgument> {
    /// <summary>
    /// <para>Sets the value of a specific property in the specified object.</para>
    /// <para>Устанавливает значение определённого свойства в указанном объекте.</para>
    /// </summary>
    /// <param name="argument">
    /// <para>The argument.</para>
    /// <para>Аргумент.</para>
    /// </param>
    /// <param name="value">
    /// <para>The value.</para>
    /// <para>Значение.</para>
    /// </param>
    virtual void Set(TArgument argument, TValue value) = 0;

    /// <summary>
    /// <para>Destroys the interface instance.</para>
    /// <para>Уничтожает экземпляр интерфейса.</para>
    /// </summary>
    virtual ~ISetter() = default;
  };
}  // namespace Platform::Interfaces
