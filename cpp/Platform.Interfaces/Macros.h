#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

namespace Platform::Interfaces {
/// <summary>
/// <para>Generates four reference-qualified accessors that cast this object to the wrapped type.</para>
/// <para>Создаёт четыре метода доступа с квалификаторами ссылки, приводящие объект к обёрнутому типу.</para>
/// </summary>
#define THIS_REFERENCE_WRAPPER_METHODS(MethodName, TWrapped)                           \
  constexpr auto&& MethodName()& { return static_cast<TWrapped&>(*this); }             \
  constexpr auto&& MethodName()&& { return static_cast<TWrapped&&>(*this); }           \
  constexpr auto&& MethodName() const& { return static_cast<const TWrapped&>(*this); } \
  constexpr auto&& MethodName() const&& { return static_cast<const TWrapped&&>(*this); }

/// <summary>
/// <para>Generates four reference-qualified accessors for a member variable.</para>
/// <para>Создаёт четыре метода доступа с квалификаторами ссылки для поля.</para>
/// </summary>
#define VARIABLE_WRAPPER_METHODS(MethodName, VariableName)      \
  constexpr auto&& MethodName()& { return VariableName; }       \
  constexpr auto&& MethodName()&& { return VariableName; }      \
  constexpr auto&& MethodName() const& { return VariableName; } \
  constexpr auto&& MethodName() const&& { return VariableName; }

/// <summary>
/// <para>Selects the next type in an extender chain.</para>
/// <para>Выбирает следующий тип в цепочке расширений.</para>
/// </summary>
#define EXTENDED_BASE_TYPE(TExtended, TExtendedBase, TExtendable, TFirstExtender, TExtenders) TFirstExtender<std::conditional_t<sizeof...(TExtenders) >= 2, TExtended<TExtendable, TExtenders...>, std::tuple_element_t<0, std::tuple<TExtenders<TExtendedBase<TExtendable>>...>>>>

/// <summary>
/// <para>Selects the next type in a decorator chain.</para>
/// <para>Выбирает следующий тип в цепочке декораторов.</para>
/// </summary>
#define DECORATED_BASE_TYPE(TDecoratedBase, TFacade, TDecorated, TFirstDecorator, TDecorators) TFirstDecorator<TFacade, std::conditional_t<sizeof...(TDecorators) >= 2, TDecoratedBase<TFacade, TDecorated, TDecorators...>, std::tuple_element_t<0, std::tuple<TDecorators<TFacade, TDecorated>...>>>>

/// <summary>
/// <para>Generates a forwarding constructor for the specified base type.</para>
/// <para>Создаёт конструктор с передачей аргументов указанному базовому типу.</para>
/// </summary>
#define USE_ALL_BASE_CONSTRUCTORS(TSelf, TBase) \
  template <typename... TParams>                \
  TSelf(TParams&&... params) : TBase(std::forward<TParams>(params)...) {}

/// <summary>
/// <para>Calls a method directly for a concrete type or virtually for an abstract type.</para>
/// <para>Вызывает метод напрямую для конкретного типа или виртуально для абстрактного типа.</para>
/// </summary>
#define DIRECT_METHOD_CALL(TClass, Object, MethodName, ...) (std::is_abstract<TClass>::value ? Object.MethodName(__VA_ARGS__) : Object.TClass::MethodName(__VA_ARGS__))

}  // namespace Platform::Interfaces
