#pragma once

#include "IProvider[TProvided, TArgument].h"
#include "ISetter[TValue, TArgument].h"

namespace Platform::Interfaces {
  template <typename...>
  struct IProperty;

  /// <summary>
  /// <para>Defines a specific property operator that is able to get or set values of that property.</para>
  /// <para>Определяет оператор определённого свойства, который может получать или устанавливать его значения.</para>
  /// </summary>
  /// <typeparam name="TObject">
  /// <para>Object type.</para>
  /// <para>Тип объекта.</para>
  /// </typeparam>
  /// <typeparam name="TValue">
  /// <para>Property value type.</para>
  /// <para>Тип значения свойства.</para>
  /// </typeparam>
  template <typename TObject, typename TValue>
  struct IProperty<TObject, TValue> : public ISetter<TValue, TObject>, IProvider<TValue, TObject> {
    virtual ~IProperty() = default;
  };
}  // namespace Platform::Interfaces
