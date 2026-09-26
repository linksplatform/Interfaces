#pragma once

#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Stores an extendable object by reference and exposes it to derived types.</para>
  /// <para>Хранит расширяемый объект по ссылке и предоставляет его производным типам.</para>
  /// </summary>
  /// <typeparam name="TExtendable">
  /// <para>The extendable object type.</para>
  /// <para>Тип расширяемого объекта.</para>
  /// </typeparam>
  template <typename TExtendable>
  class ExtendedReferenceBase {
   public:
    /// <summary>
    /// <para>Stores a reference to an extendable object.</para>
    /// <para>Сохраняет ссылку на расширяемый объект.</para>
    /// </summary>
    /// <param name="reference">
    /// <para>The object to extend.</para>
    /// <para>Расширяемый объект.</para>
    /// </param>
    ExtendedReferenceBase(TExtendable& reference) : extendable(reference) {}

    /// <summary>
    /// <para>Accesses the referenced object with the reference category of this object.</para>
    /// <para>Предоставляет доступ к объекту по ссылке с соответствующей категорией ссылки.</para>
    /// </summary>
    VARIABLE_WRAPPER_METHODS(extended, extendable)

   protected:
    /// <summary>
    /// <para>The referenced extendable object.</para>
    /// <para>Расширяемый объект по ссылке.</para>
    /// </summary>
    TExtendable& extendable;
  };
}  // namespace Platform::Interfaces
