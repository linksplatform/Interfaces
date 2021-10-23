#pragma once

#include "Macros.h"

namespace Platform::Interfaces
{
    template<typename TExtendable>
    class ExtendedBase : public TExtendable
    {
    public:
        THIS_REFERENCE_WRAPPER_METHODS(extended, TExtendable)
    };
}
