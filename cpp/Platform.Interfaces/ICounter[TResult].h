#pragma once

namespace Platform::Interfaces {
  template <typename...>
  struct ICounter;

  /// <summary>
  /// <para>Defines a counter.</para>
  /// <para>Определяет счётчик.</para>
  /// </summary>
  /// <typeparam name="TResult">
  /// <para>The count result type.</para>
  /// <para>Тип результата подсчёта.</para>
  /// </typeparam>
  template <typename TResult>
  struct ICounter<TResult> {
    /// <summary>
    /// <para>Performs a count.</para>
    /// <para>Выполняет подсчёт.</para>
    /// </summary>
    /// <returns>
    /// <para>The count result.</para>
    /// <para>Результат подсчёта.</para>
    /// </returns>
    virtual TResult Count() = 0;

    /// <summary>
    /// <para>Destroys the interface instance.</para>
    /// <para>Уничтожает экземпляр интерфейса.</para>
    /// </summary>
    virtual ~ICounter() = default;
  };
}  // namespace Platform::Interfaces
