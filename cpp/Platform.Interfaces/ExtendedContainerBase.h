#pragma once

#include "Macros.h"

namespace Platform::Interfaces
{
    template<typename TExtendable>
    class ExtendedContainerBase
    {
    public:
        VARIABLE_WRAPPER_METHODS(extended, extendable)

    protected:
        TExtendable extendable;
    };
}
