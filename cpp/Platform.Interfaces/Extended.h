#pragma once

#include "ExtendedBase.h"
#include "Macros.h"

namespace Platform::Interfaces {
  template <typename TExtendable, template <typename> typename TFirstExtender, template <typename> typename... TExtenders>
  class Extended : public EXTENDED_BASE_TYPE(Extended, ExtendedBase, TExtendable, TFirstExtender, TExtenders) {};
}  // namespace Platform::Interfaces
