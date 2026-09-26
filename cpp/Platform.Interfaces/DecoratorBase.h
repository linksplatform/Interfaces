#pragma once

#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Provides typed access to a decorated object and its facade.</para>
  /// <para>Предоставляет типизированный доступ к декорируемому объекту и его фасаду.</para>
  /// </summary>
  /// <typeparam name="TFacade">
  /// <para>The facade type.</para>
  /// <para>Тип фасада.</para>
  /// </typeparam>
  /// <typeparam name="TDecorated">
  /// <para>The decorated object type.</para>
  /// <para>Тип декорируемого объекта.</para>
  /// </typeparam>
  template <typename TFacade, typename TDecorated>
  struct DecoratorBase : public TDecorated {
    /// <summary>
    /// <para>Forwards constructor arguments to the decorated type.</para>
    /// <para>Передаёт аргументы конструктора декорируемому типу.</para>
    /// </summary>
    USE_ALL_BASE_CONSTRUCTORS(DecoratorBase, TDecorated)

    /// <summary>
    /// <para>Accesses the decorated object with the reference category of this object.</para>
    /// <para>Предоставляет доступ к декорируемому объекту с соответствующей категорией ссылки.</para>
    /// </summary>
    THIS_REFERENCE_WRAPPER_METHODS(decorated, TDecorated)
    /// <summary>
    /// <para>Accesses the facade with the reference category of this object.</para>
    /// <para>Предоставляет доступ к фасаду с соответствующей категорией ссылки.</para>
    /// </summary>
    THIS_REFERENCE_WRAPPER_METHODS(facade, TFacade)
  };
}  // namespace Platform::Interfaces
