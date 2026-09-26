#pragma once

#include "ExtendedBase.h"
#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Composes one or more extenders around an object.</para>
  /// <para>Объединяет одно или несколько расширений вокруг объекта.</para>
  /// </summary>
  /// <typeparam name="TExtendable">
  /// <para>The extendable object type.</para>
  /// <para>Тип расширяемого объекта.</para>
  /// </typeparam>
  /// <typeparam name="TFirstExtender">
  /// <para>The first extender template.</para>
  /// <para>Шаблон первого расширения.</para>
  /// </typeparam>
  /// <typeparam name="TExtenders">
  /// <para>The remaining extender templates.</para>
  /// <para>Шаблоны остальных расширений.</para>
  /// </typeparam>
  template <typename TExtendable, template <typename> typename TFirstExtender, template <typename> typename... TExtenders>
  class Extended : public EXTENDED_BASE_TYPE(Extended, ExtendedBase, TExtendable, TFirstExtender, TExtenders) {};
}  // namespace Platform::Interfaces
