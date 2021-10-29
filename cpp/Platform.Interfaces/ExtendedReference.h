#pragma once

#include "Macros.h"
#include "ExtendedReferenceBase.h"

namespace Platform::Interfaces
{
    template<typename TExtendable, template<typename> typename TFirstExtender, template<typename> typename ...TExtenders>
    class ExtendedReference : public EXTENDED_BASE_TYPE(ExtendedReference, ExtendedReferenceBase, TExtendable, TFirstExtender, TExtenders)
    {
        using Base = EXTENDED_BASE_TYPE(ExtendedReference, ExtendedReferenceBase, TExtendable, TFirstExtender, TExtenders);

    public:
        USE_ALL_BASE_CONSTRUCTORS(ExtendedReference, Base)
    };
}
