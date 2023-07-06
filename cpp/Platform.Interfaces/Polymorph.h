#pragma once

#include "Macros.h"

namespace Platform::Interfaces {
  template <typename TSelf, typename... TBase>
  class Polymorph : public TBase... {
   protected:
    THIS_REFERENCE_WRAPPER_METHODS(object, TSelf)
  };
}  // namespace Platform::Interfaces
