#pragma once

namespace Platform::Interfaces {
  template <typename...>
  struct ISetter;

  template <typename TValue>
  struct ISetter<TValue> {
    virtual void Set(TValue value) = 0;

    virtual ~ISetter() = default;
  };
}  // namespace Platform::Interfaces
