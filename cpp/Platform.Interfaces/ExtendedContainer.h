#pragma once

#include "ExtendedContainerBase.h"
#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Composes extenders around an object stored by value.</para>
  /// <para>Объединяет расширения вокруг объекта, хранимого по значению.</para>
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
  class ExtendedContainer : public EXTENDED_BASE_TYPE(ExtendedContainer, ExtendedContainerBase, TExtendable, TFirstExtender, TExtenders) {};
}  // namespace Platform::Interfaces
