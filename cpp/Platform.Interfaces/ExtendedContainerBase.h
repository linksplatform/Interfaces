#pragma once

#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Stores an extendable object by value and exposes it to derived types.</para>
  /// <para>Хранит расширяемый объект по значению и предоставляет его производным типам.</para>
  /// </summary>
  /// <typeparam name="TExtendable">
  /// <para>The extendable object type.</para>
  /// <para>Тип расширяемого объекта.</para>
  /// </typeparam>
  template <typename TExtendable>
  class ExtendedContainerBase {
   public:
    /// <summary>
    /// <para>Accesses the stored object with the reference category of this object.</para>
    /// <para>Предоставляет доступ к хранимому объекту с соответствующей категорией ссылки.</para>
    /// </summary>
    VARIABLE_WRAPPER_METHODS(extended, extendable)

   protected:
    /// <summary>
    /// <para>The stored extendable object.</para>
    /// <para>Хранимый расширяемый объект.</para>
    /// </summary>
    TExtendable extendable;
  };
}  // namespace Platform::Interfaces
