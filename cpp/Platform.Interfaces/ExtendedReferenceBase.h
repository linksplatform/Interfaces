#pragma once

#include "Macros.h"

namespace Platform::Interfaces {
  template <typename TExtendable>
  class ExtendedReferenceBase {
   public:
    ExtendedReferenceBase(TExtendable& reference) : extendable(reference) {}

    VARIABLE_WRAPPER_METHODS(extended, extendable)

   protected:
    TExtendable& extendable;
  };
}  // namespace Platform::Interfaces
