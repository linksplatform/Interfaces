#pragma once

namespace Platform::Interfaces {
  template <typename...>
  struct IMatcher;

  template <typename TArgument>
  struct IMatcher<TArgument> {
    virtual bool IsMatched(TArgument argument) = 0;

    virtual ~IMatcher() = default;
  };
}  // namespace Platform::Interfaces
