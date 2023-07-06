#pragma once

#include "ExtendedContainerBase.h"
#include "Macros.h"

namespace Platform::Interfaces {
  template <typename TExtendable, template <typename> typename TFirstExtender, template <typename> typename... TExtenders>
  class ExtendedContainer : public EXTENDED_BASE_TYPE(ExtendedContainer, ExtendedContainerBase, TExtendable, TFirstExtender, TExtenders) {};
}  // namespace Platform::Interfaces
