#pragma once

namespace Platform::Interfaces {
  template <typename...>
  struct IMatcher;

  template <typename TCandidate>
  struct IMatcher<TCandidate> {
    virtual bool IsMatched(TCandidate candidate) = 0;

    virtual ~IMatcher() = default;
  };
}  // namespace Platform::Interfaces
