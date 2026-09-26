#pragma once

#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Provides typed access to the object being extended.</para>
  /// <para>Предоставляет типизированный доступ к расширяемому объекту.</para>
  /// </summary>
  /// <typeparam name="TExtendable">
  /// <para>The extendable object type.</para>
  /// <para>Тип расширяемого объекта.</para>
  /// </typeparam>
  template <typename TExtendable>
  struct ExtendedBase : public TExtendable {
    /// <summary>
    /// <para>Accesses the extended object with the reference category of this object.</para>
    /// <para>Предоставляет доступ к расширяемому объекту с соответствующей категорией ссылки.</para>
    /// </summary>
    THIS_REFERENCE_WRAPPER_METHODS(extended, TExtendable)
  };
}  // namespace Platform::Interfaces
