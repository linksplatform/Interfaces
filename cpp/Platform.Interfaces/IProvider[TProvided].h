#pragma once

namespace Platform::Interfaces {
  template <typename...>
  struct IProvider;

  template <typename TProvided>
  struct IProvider<TProvided> {
    virtual TProvided Get() = 0;

    virtual ~IProvider() = default;
  };
}  // namespace Platform::Interfaces
