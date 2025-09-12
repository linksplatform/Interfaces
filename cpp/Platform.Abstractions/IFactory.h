#pragma once

namespace Platform::Abstractions {
  template <typename...>
  struct IFactory;

  template <typename TProduct>
  struct IFactory<TProduct> {
    virtual TProduct Create() = 0;

    virtual ~IFactory() = default;
  };
}  // namespace Platform::Abstractions
