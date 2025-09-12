#pragma once

namespace Platform::Abstractions {
  template <typename...>
  struct IProvider;

  template <typename TProvided, typename TArgument>
  struct IProvider<TProvided, TArgument> {
    virtual TProvided Get(TArgument argument) = 0;

    virtual ~IProvider() = default;
  };
}  // namespace Platform::Abstractions
