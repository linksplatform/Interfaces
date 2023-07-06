#pragma once

namespace Platform::Interfaces {
  template <typename...>
  struct IFactory;

  template <typename TProduct>
  struct IFactory<TProduct> {
    virtual TProduct Create() = 0;

    virtual ~IFactory() = default;
  };
}  // namespace Platform::Interfaces
