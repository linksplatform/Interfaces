#pragma once

#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Builds the decorator chain for a facade and its decorated object.</para>
  /// <para>Создаёт цепочку декораторов для фасада и декорируемого объекта.</para>
  /// </summary>
  /// <typeparam name="TFacade">
  /// <para>The facade type.</para>
  /// <para>Тип фасада.</para>
  /// </typeparam>
  /// <typeparam name="TDecorated">
  /// <para>The decorated object type.</para>
  /// <para>Тип декорируемого объекта.</para>
  /// </typeparam>
  /// <typeparam name="TFirstDecorator">
  /// <para>The first decorator template.</para>
  /// <para>Шаблон первого декоратора.</para>
  /// </typeparam>
  /// <typeparam name="TDecorators">
  /// <para>The remaining decorator templates.</para>
  /// <para>Шаблоны остальных декораторов.</para>
  /// </typeparam>
  template <typename TFacade, typename TDecorated, template <typename, typename> typename TFirstDecorator, template <typename, typename> typename... TDecorators>
  struct DecoratedBase : public DECORATED_BASE_TYPE(DecoratedBase, TFacade, TDecorated, TFirstDecorator, TDecorators) {
    /// <summary>
    /// <para>The base type of the decorator chain.</para>
    /// <para>Базовый тип цепочки декораторов.</para>
    /// </summary>
    using base = DECORATED_BASE_TYPE(DecoratedBase, TFacade, TDecorated, TFirstDecorator, TDecorators);

    /// <summary>
    /// <para>Forwards constructor arguments to the next decorator.</para>
    /// <para>Передаёт аргументы конструктора следующему декоратору.</para>
    /// </summary>
    USE_ALL_BASE_CONSTRUCTORS(DecoratedBase, base)
  };
}  // namespace Platform::Interfaces
