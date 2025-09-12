#pragma once

namespace Platform::Abstractions {
  template <typename...>
  struct ICriterionMatcher;

  template <typename TArgument>
  struct ICriterionMatcher<TArgument> {
    virtual bool IsMatched(TArgument argument) = 0;

    virtual ~ICriterionMatcher() = default;
  };
}  // namespace Platform::Abstractions
