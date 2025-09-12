#pragma once

#include "Macros.h"

namespace Platform::Abstractions {
  template <typename TExtendable>
  struct ExtendedBase : public TExtendable {
    THIS_REFERENCE_WRAPPER_METHODS(extended, TExtendable)
  };
}  // namespace Platform::Abstractions
