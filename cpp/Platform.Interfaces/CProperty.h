#pragma once

#include "CProvider.h"
#include "CSetter.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires an operator that gets and sets one property of an object.</para>
  /// <para>Требует оператор, получающий и устанавливающий одно свойство объекта.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TObject">
  /// <para>The object type.</para>
  /// <para>Тип объекта.</para>
  /// </typeparam>
  /// <typeparam name="TValue">
  /// <para>The value type.</para>
  /// <para>Тип значения.</para>
  /// </typeparam>
  template <typename TSelf, typename TObject, typename TValue>
  concept CProperty = CSetter<TSelf, TValue, TObject> && CProvider<TSelf, TValue, TObject>;
}
