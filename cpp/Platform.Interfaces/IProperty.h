#pragma once

#include "IProvider[TProvided, TArgument].h"
#include "ISetter[TValue, TArgument].h"

namespace Platform::Interfaces {
  template <typename...>
  struct IProperty;

  template <typename TObject, typename TValue>
  struct IProperty<TObject, TValue> : public ISetter<TValue, TObject>, IProvider<TValue, TObject> {
    virtual ~IProperty() = default;
  };
}  // namespace Platform::Interfaces
