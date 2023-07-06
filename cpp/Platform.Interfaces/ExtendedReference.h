#pragma once

#include "ExtendedReferenceBase.h"
#include "Macros.h"

namespace Platform::Interfaces {
  template <typename TExtendable, template <typename> typename TFirstExtender, template <typename> typename... TExtenders>
  class ExtendedReference : public EXTENDED_BASE_TYPE(ExtendedReference, ExtendedReferenceBase, TExtendable, TFirstExtender, TExtenders) {
    using base = EXTENDED_BASE_TYPE(ExtendedReference, ExtendedReferenceBase, TExtendable, TFirstExtender, TExtenders);

   public:
    USE_ALL_BASE_CONSTRUCTORS(ExtendedReference, base)
  };
}  // namespace Platform::Interfaces
