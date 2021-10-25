#pragma once

#include "Macros.h"
#include "ExtendedBase.h"

namespace Platform::Interfaces
{
    template<typename TExtendable, template<typename> typename TFirstExtender, template<typename> typename ...TExtenders>
    struct Extended : public EXTENDED_BASE_TYPE(Extended, ExtendedBase, TExtendable, TFirstExtender, TExtenders)
    {
    };
}
