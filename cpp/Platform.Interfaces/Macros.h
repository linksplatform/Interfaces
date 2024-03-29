#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

namespace Platform::Interfaces {
#define THIS_REFERENCE_WRAPPER_METHODS(MethodName, TWrapped)                           \
  constexpr auto&& MethodName()& { return static_cast<TWrapped&>(*this); }             \
  constexpr auto&& MethodName()&& { return static_cast<TWrapped&&>(*this); }           \
  constexpr auto&& MethodName() const& { return static_cast<const TWrapped&>(*this); } \
  constexpr auto&& MethodName() const&& { return static_cast<const TWrapped&&>(*this); }

#define VARIABLE_WRAPPER_METHODS(MethodName, VariableName)      \
  constexpr auto&& MethodName()& { return VariableName; }       \
  constexpr auto&& MethodName()&& { return VariableName; }      \
  constexpr auto&& MethodName() const& { return VariableName; } \
  constexpr auto&& MethodName() const&& { return VariableName; }

#define EXTENDED_BASE_TYPE(TExtended, TExtendedBase, TExtendable, TFirstExtender, TExtenders) TFirstExtender<std::conditional_t<sizeof...(TExtenders) >= 2, TExtended<TExtendable, TExtenders...>, std::tuple_element_t<0, std::tuple<TExtenders<TExtendedBase<TExtendable>>...>>>>

#define DECORATED_BASE_TYPE(TDecoratedBase, TFacade, TDecorated, TFirstDecorator, TDecorators) TFirstDecorator<TFacade, std::conditional_t<sizeof...(TDecorators) >= 2, TDecoratedBase<TFacade, TDecorated, TDecorators...>, std::tuple_element_t<0, std::tuple<TDecorators<TFacade, TDecorated>...>>>>

#define USE_ALL_BASE_CONSTRUCTORS(TSelf, TBase) \
  template <typename... TParams>                \
  TSelf(TParams&&... params) : TBase(std::forward<TParams>(params)...) {}

#define DIRECT_METHOD_CALL(TClass, Object, MethodName, ...) (std::is_abstract<TClass>::value ? Object.MethodName(__VA_ARGS__) : Object.TClass::MethodName(__VA_ARGS__))

}  // namespace Platform::Interfaces
