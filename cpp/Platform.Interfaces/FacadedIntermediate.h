#pragma once

#include "Macros.h"

namespace Platform::Interfaces
{
  template<typename TFacaded, typename TExtendable, template<typename, typename> typename TFirstExtender, template<typename, typename> typename ...TExtenders>
  struct FacadedIntermediate : public FACADED_BASE_TYPE(FacadedIntermediate, TFacaded, TExtendable, TFirstExtender, TExtenders)
  {
      using base = FACADED_BASE_TYPE(FacadedIntermediate, TFacaded, TExtendable, TFirstExtender, TExtenders);

      USE_ALL_BASE_CONSTRUCTORS(FacadedIntermediate, base)
  };
}