#pragma once

namespace Platform::Abstractions {
  template <typename...>
  struct ICounter;

  template <typename TResult>
  struct ICounter<TResult> {
    virtual TResult Count() = 0;

    virtual ~ICounter() = default;
  };
}  // namespace Platform::Abstractions
