#pragma once

namespace Platform::Abstractions {
  template <typename...>
  struct ICounter;

  template <typename TResult, typename TArgument>
  struct ICounter<TResult, TArgument> {
    virtual TResult Count(TArgument argument) = 0;

    virtual ~ICounter() = default;
  };
}  // namespace Platform::Abstractions
