#pragma once

#include "Macros.h"

namespace Platform::Interfaces
{
    template<typename TFacade, typename TExtendable>
    struct FacadedBase : public TExtendable
    {
        USE_ALL_BASE_CONSTRUCTORS(FacadedBase, TExtendable)

        THIS_REFERENCE_WRAPPER_METHODS(extended, TExtendable)
        THIS_REFERENCE_WRAPPER_METHODS(facade, TExtendable)
    };
}
