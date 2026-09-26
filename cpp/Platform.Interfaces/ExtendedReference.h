#pragma once

#include "ExtendedReferenceBase.h"
#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Composes extenders around an object stored by reference.</para>
  /// <para>Объединяет расширения вокруг объекта, хранимого по ссылке.</para>
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
  class ExtendedReference : public EXTENDED_BASE_TYPE(ExtendedReference, ExtendedReferenceBase, TExtendable, TFirstExtender, TExtenders) {
    /// <summary>
    /// <para>The composed extender base type.</para>
    /// <para>Тип базовой цепочки расширений.</para>
    /// </summary>
    using base = EXTENDED_BASE_TYPE(ExtendedReference, ExtendedReferenceBase, TExtendable, TFirstExtender, TExtenders);

   public:
    /// <summary>
    /// <para>Forwards constructor arguments to the extender chain.</para>
    /// <para>Передаёт аргументы конструктора цепочке расширений.</para>
    /// </summary>
    USE_ALL_BASE_CONSTRUCTORS(ExtendedReference, base)
  };
}  // namespace Platform::Interfaces
