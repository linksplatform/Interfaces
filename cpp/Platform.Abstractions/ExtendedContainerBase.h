#pragma once

#include "Macros.h"

namespace Platform::Abstractions {
  template <typename TExtendable>
  class ExtendedContainerBase {
   public:
    VARIABLE_WRAPPER_METHODS(extended, extendable)

   protected:
    TExtendable extendable;
  };
}  // namespace Platform::Abstractions
