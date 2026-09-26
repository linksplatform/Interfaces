#pragma once

#include "Macros.h"

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Composes one or more decorators around an object.</para>
  /// <para>Объединяет один или несколько декораторов вокруг объекта.</para>
  /// </summary>
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
  template <typename TDecorated, template <typename, typename> typename TFirstDecorator, template <typename, typename> typename... TDecorators>
  struct Decorated : public DecoratedBase<Decorated<TDecorated, TFirstDecorator, TDecorators...>, TDecorated, TFirstDecorator, TDecorators...> {
    /// <summary>
    /// <para>The composed decorator base type.</para>
    /// <para>Тип базовой цепочки декораторов.</para>
    /// </summary>
    using base = DecoratedBase<Decorated<TDecorated, TFirstDecorator, TDecorators...>, TDecorated, TFirstDecorator, TDecorators...>;

    /// <summary>
    /// <para>Forwards constructor arguments to the decorator chain.</para>
    /// <para>Передаёт аргументы конструктора цепочке декораторов.</para>
    /// </summary>
    USE_ALL_BASE_CONSTRUCTORS(Decorated, base)
  };
}  // namespace Platform::Interfaces
