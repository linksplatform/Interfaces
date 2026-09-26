#pragma once

#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Combines base types and exposes the derived object to them.</para>
  /// <para>Объединяет базовые типы и предоставляет им доступ к производному объекту.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
  /// <typeparam name="TBase">
  /// <para>The base types.</para>
  /// <para>Базовые типы.</para>
  /// </typeparam>
  template <typename TSelf, typename... TBase>
  class Polymorph : public TBase... {
   protected:
    /// <summary>
    /// <para>Accesses the derived object with the reference category of this object.</para>
    /// <para>Предоставляет доступ к производному объекту с соответствующей категорией ссылки.</para>
    /// </summary>
    THIS_REFERENCE_WRAPPER_METHODS(object, TSelf)
  };
}  // namespace Platform::Interfaces
